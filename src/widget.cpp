#include "widget.h"
#include"vector"
#include<opencv2/opencv.hpp>
//#include<opencv2/highgui/highgui.hpp>
QPointF my_center;
bool merge_mode = false;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //绑定点选事件
    for(QRadioButton *bt:ui->groupBox->findChildren<QRadioButton*>()){
        connect(bt,&QRadioButton::clicked,this,&Widget::checkWho);
    }
    //设置画布大小与格式(带透明度的32位RGB彩色图)
    image = QImage(1920,1080,QImage::Format_ARGB32);
    //填充成透明颜色
    image.fill(QColor(0,0,0,0));
    penColor = QColor(255,0,0);    //设置画笔基础颜色
    penWidth = 5;                  //设置画笔宽度
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::event(QEvent *event)
{
    QMouseEvent *e = static_cast<QMouseEvent*>(event);

    if(event->type()==QEvent::MouseButtonPress){
        QPoint press = e->pos();
        first = last = press;

        //if(num!=5){
        path = QPainterPath(first);             // 落笔处(落笔点)
        shapeAndPath shapePath(num,path);       // 创建一个封装了 图形形状 和 图形路径 的类
        paths.push(shapePath);                  // 将上面的类加入到绘画栈中
        //}

        pressedshapeAndPathIndex=0;
        for(shapeAndPath& sp:paths){
            if(sp.value().controlPointRect().contains(press) && sp.key()!=5)
                break;
            pressedshapeAndPathIndex++;
        }
    }
    else if(event->type()==QEvent::MouseMove){
        before = last;                          //往后移 保持记录倒数第二个点
        last = e->pos();                        //往后移 保持记录最后一个点
        //如果没有操作当前正在被编辑的图像，那是一个移动
        if(pressedshapeAndPathIndex<paths.size()-1){
            //平移
            /*
            bool ok = false;
            QPainterPath cur_path = paths[pressedshapeAndPathIndex].value();
            for(int i=0;i<paths.size();i++){
                if(i!=pressedshapeAndPathIndex && paths[i].value().intersects(cur_path)){
                    paths[pressedshapeAndPathIndex].value().addPath(paths[i].value());


                     update();
                    paths.removeAt(i);
                    ok = true;
                    break;
                }
            }
            Widget::setPenStyle(QColor("purple"),10);
            painter.drawPolygon(paths[pressedshapeAndPathIndex].value().toFillPolygon());
            update();
            if(ok) qDebug()<<"successed";
            else qDebug()<<"G";
            */

            paths[pressedshapeAndPathIndex].value().translate(last-before);
            qDebug()<<QString("current idx is ")+QString::number(pressedshapeAndPathIndex);
        }
        else
            Widget::fixFinalPath(num,path);     //确定最终的图形 num表示图形样式 path为图形路径
        update();
    }
    else if(event->type()==QEvent::MouseButtonRelease){
        qDebug()<<path.length();
        //如果是当前正在被操作的对象
        if(pressedshapeAndPathIndex>=paths.size()-1){
            if(!paths.isEmpty()) {
                if(num!=5)
                    Widget::drawPathtoDevice(&image,path);
                else
                    Widget::eraserPathfromDevice(pressedshapeAndPathIndex, &image,path);
                if(num<4){                      //基础图形是直接换成path
                    paths.last().value()=path;
                } else if(num!=5){
                     bool intersects =false;
                    if(num==4){

                        for(int i=0;i<paths.size();i++){
                            if(paths[i].value().intersects(path.boundingRect())){
                                intersects = true;
                                paths[i].value().addPath(path);
                                //paths[i].value() += path;
                                break;
                            }
                        }
                    }
                    if(!intersects){
                    //路径图形要加上之前的path
                    paths.last().value().addPath(path);
                    pointsNum = 1;
                    }
                }
            }
            path.clear();                       //松笔，代表图形已确定，清空临时path
            QPoint size = last-first;           //点击与松开的间隔太小时，视作失误点击，撤销操作
            if((abs(size.x())<7&&abs(size.y())<7&&num<4)) paths.pop_back();
        }
        else {
            Widget::repaintImage();
            paths.pop_back();
        }
        update();
    }
    if(event->type()==QEvent::Paint){
        painter.begin(this);
        painter.drawImage(0,0,image);           //先画已经确定的image图
        painter.end();
        if(!path.isEmpty())                     //再画当前正在确定的path （画在主界面上）
        {
            if(num!=5)
                drawPathtoDevice(this,path);
            else{
                //eraserPathfromDevice(pressedshapeAndPathIndex,this,path);
            }
        }
        if(pressedshapeAndPathIndex<paths.size()){
            penColor = QColor(0,255,0);
            drawPathtoDevice(this,paths[pressedshapeAndPathIndex].value());
            penColor = QColor(255,0,0);
        }
    }
    if(event->type()==QEvent::KeyPress){
        if(merge_mode){
        qDebug()<<"merge ";
        int pressedshapeAndPathIndex1 = 0;
        int pressedshapeAndPathIndex2 = 1;
        if(paths.size()>1){
            paths[pressedshapeAndPathIndex1].value() += (paths[pressedshapeAndPathIndex2].value());
            paths.removeAt(pressedshapeAndPathIndex2);
        }
        }
    }
    return QWidget::event(event);
}

void Widget::checkWho()
{
    this->num = ((QRadioButton*)QObject::sender())->whatsThis().toInt();
    qDebug()<<num;
}

void Widget::setPenStyle(const QColor &color, int width)
{
    QPen pen(color);
    pen.setWidth(width);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing, true);
}

void Widget::on_pushButton_clicked()
{
    if(!paths.isEmpty())            //删除最后一条路径
        paths.pop_back();
    Widget::repaintImage();
}

void Widget::drawPathtoDevice(QPaintDevice *device,QPainterPath &path)
{
    painter.begin(device);
    Widget::setPenStyle(penColor,penWidth);
    painter.drawPath(path);
    painter.end();
}

void Widget::fixFinalPath(int num,QPainterPath &path)
{
    switch (num) {
    case 1:
        path.clear();
        path.addRect(QRect(first,last).normalized());
        break;
    case 2:
        path.clear();
        path.addEllipse(QRect(first,last).normalized());
        break;
    case 3:
        path.clear();
        path = QPainterPath(first);
        path.lineTo(last);
        break;
    case 4: //既然画笔工具时 鼠标路径就是最终路径，为什么直接把路径画到Image里，而还要用path存储？
            //那是因为撤回功能，撤回需要知道撤回哪个路径，所以必须记录路径不可。
        path.lineTo(last);  //记录在路径中
        pointsNum++;        //记录点的次数+1
        if(pointsNum>255){  //每个路径记录了255个任意路径后便清零，保持绘画效率
            drawPathtoDevice(&image,path);  //记录满了便直接把路径画入 image同时清空临时路径path
            paths.last().value().addPath(path); //添加到栈顶路径中
            path.clear();
            path = QPainterPath(last);      //重新定位到最后一个点，继续画
            pointsNum=1;                    //重新记录点个数
        }
        break;
    case 5:
        path.lineTo(last);  //记录在路径中
        pointsNum++;        //记录点的次数+1
        if(pointsNum>511){  //每个路径记录了255个任意路径后便清零，保持绘画效率
            eraserPathfromDevice(pressedshapeAndPathIndex,&image,path);  //记录满了便直接把路径画入 image同时清空临时路径path
            //paths.last().value().addPath(path); //添加到栈顶路径中
            path.clear();
            path = QPainterPath(last);      //重新定位到最后一个点，继续画
            pointsNum=1;                    //重新记录点个数
        }
       break;
    }
}

void Widget::repaintImage()
{
    image.fill(QColor(0,0,0,0));    //清空图片
    //再把记录的所有路径重新画到image上
    for(auto &m:paths){
        if(m.key()!=5)
            drawPathtoDevice(&image,m.value());
        else{
            //eraserPathfromDevice(,&image,m.value());
        }
    }
    update();
}

QPointF my_Intersect(const QPainterPath &contour, const QPainterPath &line)
{
    qreal len = line.length();
    QPointF p = line.pointAtPercent(0);
    bool intersected = false;

    QPainterPath inter_path;

    for(int k = 1; k <= len; ++k)
    {
        if(contour.contains(p) && !intersected)
        {
            intersected = true;
            inter_path.moveTo(p);
        }
        else if(!contour.contains(p) && intersected)
        {

            inter_path.lineTo(p);
            return p;
        }


        qreal per = line.percentAtLength(k);
        p = line.pointAtPercent(per);
    }
    return QPointF(0,0);
}

std::vector<cv::Point> get_points_from_path(const QPainterPath &contour){
    std::vector<cv::Point> points;
    qreal len = contour.length();
    for(int k=0;k<=len;k++){
        QPointF cur = contour.pointAtPercent(contour.percentAtLength(k));
        points.push_back(cv::Point(cur.x(),cur.y()));

    }
    return points;

}
QPointF my_Intersect2(const QPainterPath & contour, const QPainterPath &line){

    qreal len = line.length();
    QPointF p=line.pointAtPercent(0);

    for(int k=1;k<=len;k++){

        if(contour.contains(p)){
            return p;
        }

        qreal per = line.percentAtLength(k);
       // qDebug()<<"per "<<QString::number(per);
        p = line.pointAtPercent(per);

    }
    return p;
}

QVector<QPointF> get_cutting_line(const QPainterPath& coutour, const QPainterPath &line){

    QVector<QPointF> cutting_line_points;
    cutting_line_points.push_back(my_Intersect2(coutour,line));
    cutting_line_points.push_back(my_Intersect2(coutour,line.toReversed()));
    return cutting_line_points;

}

void Widget::eraserPathfromDevice(int pressedshapeAndPathIndex,QPaintDevice *device,QPainterPath &path)
{
    painter.begin(device);
    pressedshapeAndPathIndex = -1;
    for(int i=0;i<paths.size();i++){
        if(paths[i].value().controlPointRect().intersects(path.boundingRect())){
            pressedshapeAndPathIndex = i;
            break;
        }
    }
    if(pressedshapeAndPathIndex == -1) {
        qDebug()<<"invalid operation";
        return;
    }
    qDebug()<<"current pressed "<<QString::number(pressedshapeAndPathIndex);
    //painter.setCompositionMode(QPainter::CompositionMode_Clear);

    Widget::setPenStyle(QColor(),10);
    painter.drawPath(path);

    //QPainterPath another_one = paths[0].value();
    QRectF bounding_rect = paths[pressedshapeAndPathIndex].value().boundingRect();

    my_center = bounding_rect.center();


   //findcontours
    //QPolygonF polygon = paths[0].value().boundingRect();

    QList<QLineF> edgeList;
    QPainterPath original_path = paths[pressedshapeAndPathIndex].value();



    QPainterPathStroker s;
    QPainterPath contour = s.createStroke(paths[pressedshapeAndPathIndex].value()-path);
   // painter.drawPath(contour);
    QPolygonF polygon = contour.toFillPolygon();


    for(int i=0;i<polygon.count();i++){
        QLineF edge;
        if(i!=(polygon.count()-1)){
            edge.setPoints(polygon.at(i), polygon.at(i+1));
        }
        else{
            edge.setPoints(polygon.at(i),polygon.at(0));
        }
        edgeList<<edge;
    }

  for(int i=0;i<edgeList.size();i++){
    Widget::setPenStyle(QColor("purple"),10);
        QPointF AA = my_Intersect(paths[0].value(),path);
        //painter.drawPoint(AA);
    }

  QVector<QPointF> tmp = get_cutting_line(original_path,path);

       qDebug()<<"size = " + QString::number(tmp.size());


    //painter.drawText(QPointF(500,500),QString::number(edgeList.size()));

    QList<QPointF> p1,p2;
    QPointF A=tmp[0],B=tmp[1];


 Widget::setPenStyle(QColor("blue"),10);
    for(int i=0;i<polygon.count();i++){
        qreal crossproduct = crossProduct(A,B,polygon.at(i));
        if(paths[pressedshapeAndPathIndex].key()==1){
            if(path.boundingRect().contains(polygon.at(i))){
             // painter.drawRect(path.boundingRect());
            //painter.drawPoint(polygon.at(i));
                p1 << polygon.at(i);
            }
        }

        if(crossproduct>=0){
            p1<<polygon.at(i);
        }
        if(crossproduct<=0){
            p2<<polygon.at(i);
        }
        //painter.drawPoint(polygon.at(i));
    }

   //painter.drawPolygon(polygon);

    //polygon.translate(10,10);
    QPainterPath path1 = generatePath(p1,QPolygonF(bounding_rect));
    QPainterPath path2 = generatePath(p2,QPolygonF(bounding_rect));

  //painter.drawPath(s.createStroke(paths[0].value()));
  //  painter.drawPath(paths[0].value().intersected(path));



    // painter.drawPath(path1);


//cv::Mat Img  = cv::imread("C:\\Users\\mech-mind_ljq\\Desktop\\刘洁琼-研发部-409.JPG");
//imshow("a.bmp",Img);

int L=0,W=0;
std::vector<cv::Point> points = get_points_from_path(contour);
for(int i=0;i<points.size();i++){
    W=fmax(L,points[i].x);
    L=fmax(W,points[i].y);
}


//GaussianBlur(img,img,cv::Size(3,3),0);
//cv::Mat image2;
//cv::cvtColor(img,image2,cv::COLOR_BGR2GRAY);
//Canny(image2,image2,100,250);
//std::vector<std::vector<cv::Point>> contours(10000);
//std::vector<cv::Vec4i> hierarchy(10000);
//findContours(image2,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE,cv::Point());
//cv::Mat imageContours = cv::Mat::zeros(img.size(),CV_8UC1);
//cv::Mat Contours = cv::Mat::zeros(img.size(),CV_8UC1);  //绘制
//for(int i=0;i<contours.size();i++)
//{
//    //contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数
//    for(int j=0;j<contours[i].size();j++)
//    {
//        //绘制出contours向量内所有的像素点
//        cv::Point P=cv::Point(contours[i][j].x,contours[i][j].y);
//        Contours.at<uchar>(P)=255;
//    }

//    //输出hierarchy向量内容
//    char ch[256];
//    sprintf(ch,"%d",i);
//    std::string str=ch;
//    //std::cout<<"向量hierarchy的第" <<str<<" 个元素内容为："<<std::endl<<hierarchy[i]<<std::endl<<std::endl;

//    //绘制轮廓
//    drawContours(imageContours,contours,i,cv::Scalar(255),1,8,hierarchy);
//}
//imshow("Contours Image",imageContours); //轮廓
//imshow("Point of Contours",Contours);   //向量contours内保存的所有轮廓点集
//cv::waitKey(0);



//std::vector<std::vector<cv::Point>> contours;
//std::vector<cv::Vec4i> hierarchy;

  // findContours(im,contours,hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

// cv::findContours(Img, contours,hierarchy,0,0);

   //  Widget::setPenStyle(QColor("purple"),10);
    // painter.drawPath(path2);
   // painter.drawPolygon(path1.toFillPolygon());


     //painter.drawPolygon(path2.toFillPolygon());

    //painter.drawPolygon(paths[0].value().toFillPolygon());

    //painter.drawPolygon(paths[0].value().toFillPolygons()[0]);
   // paths[0].value() = paths[0].value()&path;
    //painter.drawPath(paths[0].value());

    //paths[0].value() = path1;
    //drawPathtoDevice(this,path1);
    //drawPathtoDevice(this, path2);

    update();

    painter.end();
   shapeAndPath shapePath1(3,path1);       // 创建一个封装了 图形形状 和 图形路径 的类
  paths.push(shapePath1);                  // 将上面的类加入到绘画栈中
  shapeAndPath shapePath2(3,path2);
    paths.push(shapePath2);
paths.remove(pressedshapeAndPathIndex);

}



qreal Widget::crossProduct(QPointF A,QPointF B, QPointF C){
    return (B.x()-A.x())*(C.y()-A.y()) - (C.x()-A.x())*(B.y()-A.y());
}

qreal Widget::getDistance(QPointF A, QPointF B){
    return (A.x()-B.x())*(A.x()-B.x())+(A.y()-B.y())*(A.y()-B.y());
}
bool Widget::cmp(QPointF A, QPointF B){
    QPointF C = my_center;
    if(crossProduct(C,A,B)==0) {
        return A.x()>B.x();
    }
    else return crossProduct(C,A,B)>0;
}

QPointF Widget::getCenter(QList<QPointF> list){
    QPointF center_point(0,0);
    QPointF A,B;
    qreal max_dis = 0;
    for(int i=0;i<list.size();i++){
        for(int j=i+1;j<list.size();j++){
            if(getDistance(list[i],list[j])>max_dis){
                max_dis = getDistance(list[i],list[j]);
                A = list[i];
                B = list[j];
            }
        }
    }
    return QPointF((A.x()+B.x())/2.0,(A.y()+B.y())/2.0);
}

QPainterPath Widget::generatePath(QList<QPointF> list,QPolygonF polygon){
    my_center = getCenter(list);
    qSort(list.begin(),list.end(),cmp);
    if(list.size()==0) return QPainterPath();
    QPainterPath res(list[0]);
    for(int i=1;i<list.size();i++){
        //if(polygon.contains(list[i]))
        res.lineTo(list[i]);
    }
    res.closeSubpath();
    return res;
}
