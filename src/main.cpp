#include "widget.h"
#include<opencv2/opencv.hpp>
#include"mainwindow.h"
#include <QApplication>
#include<opencv2/highgui/highgui.hpp>



int main(int argc, char *argv[])
{


//     cv::Mat imageSource=cv::imread("C:/Users/mech-mind_ljq/Desktop/1.png");
//     imshow("Source Image",imageSource);
//     cv::Mat image;
//     GaussianBlur(imageSource,image,cv::Size(3,3),0);
//     cv::Mat image2;
//     cv::cvtColor(image,image2,cv::COLOR_BGR2GRAY);
//      //Canny(image2,image2,100,250);
//     std::vector<std::vector<cv::Point>> contours(10000);
//    std::vector<cv::Vec4i> hierarchy(10000);
//    findContours(image2,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE,cv::Point());
//     cv::Mat imageContours=cv::Mat::zeros(image.size(),CV_8UC1);
//     cv::Mat Contours=cv::Mat::zeros(image.size(),CV_8UC1);  //绘制
//     for(int i=0;i<contours.size();i++)
//     {
//         //contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数
//         for(int j=0;j<contours[i].size();j++)
//         {
//             //绘制出contours向量内所有的像素点
//             cv::Point P=cv::Point(contours[i][j].x,contours[i][j].y);
//             Contours.at<uchar>(P)=255;
//         }

//         //输出hierarchy向量内容
//         char ch[256];
//         sprintf(ch,"%d",i);
//         std::string str=ch;
//         //std::cout<<"向量hierarchy的第" <<str<<" 个元素内容为："<<std::endl<<hierarchy[i]<<std::endl<<std::endl;

//         //绘制轮廓
//         drawContours(imageContours,contours,i,cv::Scalar(255),1,8,hierarchy);
//     }
//     imshow("Contours Image",imageContours); //轮廓
//     imshow("Point of Contours",Contours);   //向量contours内保存的所有轮廓点集
//     cv::waitKey(0);

//     return 0;




    cv::Mat img = cv::imread("C:/Users/mech-mind_ljq/Desktop/R-C.PNG");
   // cv::imshow("test",img);
 QApplication a(argc, argv);
    Widget w;
    w.show();
    MainWindow ww;
    ww.show();

  cv::waitKey(0);
    return a.exec();
}
