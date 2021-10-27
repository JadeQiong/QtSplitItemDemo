#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainterPath>
#include <QImage>
#include <QPainter>
#include <QStack>
#include <QMap>

#include "ui_widget.h"
#include <QMouseEvent>
#include <QDebug>
#include <QRadioButton>
#include<QPainterPathStroker>
#include <QtMath>
#include<algorithm>

//#include "core/core.hpp"
//#include "highgui/highgui.hpp"
//#include "imgproc/imgproc.hpp"
//#include"opencv2/imgcodecs/imgcodecs.hpp"
//#include"opencv2/imgcodecs.hpp"
//#include "opencv2/world.hpp"

#include <opencv2/opencv.hpp>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
class shapeAndPath{
private:
    uint8_t n;
    QPainterPath path;
    //int _id;
public:
    shapeAndPath(uint8_t n,QPainterPath path):n(n),path(path){}
    //shapeAndPath(uint8_t n,QPainterPath path, int id):n(n),path(path),_id(id){};
    int key(){return n;}
    //int id(){return _id;}
    QPainterPath &value(){return path;}

};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:
    bool event(QEvent *event) override;
private slots:
    void checkWho();
    void on_pushButton_clicked();

private:
    void setPenStyle(const QColor &color, int width);
    void drawPathtoDevice(QPaintDevice *device,QPainterPath &path);
    void eraserPathfromDevice(int pressedshapeAndPathIndex, QPaintDevice *device,QPainterPath &path);
    void fixFinalPath(int num,QPainterPath &path);
    void repaintImage();

    //add
    static qreal crossProduct(QPointF A,QPointF B, QPointF C);
    static bool cmp(QPointF A, QPointF B);
    QPainterPath generatePath(QList<QPointF> list,QPolygonF polygon);
    static QPointF getCenter(QList<QPointF> list);
    static qreal getDistance(QPointF A, QPointF B);
private:
    Ui::Widget *ui;
    QImage image;
    int num = 1;
    int pointsNum=1;            //记录点的个数
    QPoint first,before,last;
    QPainter painter;
    QPainterPath path;          //临时绘画路径
    int pressedshapeAndPathIndex=3;
    QStack<shapeAndPath> paths; //绘画路径栈
    QColor penColor;
    uint8_t penWidth;

    //static QPointF center;
};
#endif // WIDGET_H
