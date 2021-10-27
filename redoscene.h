#pragma once
#include<QGraphicsScene>
#include"redoitems.h"
QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QObject;
class QGraphicsViewItem;

QT_END_NAMESPACE

class redoscene:public QGraphicsScene
{
    Q_OBJECT
public:
    redoscene(QObject* parent =0);
    ~redoscene(){};
 signals:
    void itemMoved(redoitems *moveItem,const QPointF& movedFromPostion);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
private:
    QGraphicsItem* movingItem;
    QPointF oldPos;
};

