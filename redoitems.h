#pragma once
#include<QGraphicsPolygonItem>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QPointF;
QT_END_NAMESPACE


class redoitems:public QGraphicsPolygonItem
{
public:
    enum{Type = UserType+1};
    enum redoitem_type{Box, Triangle};
    explicit redoitems(redoitem_type redoitemType, QGraphicsItem *item=0);
    redoitem_type get_redoitems()const{
        return polygon()==boxPolygon? Box:Triangle;
    }
   // virtual void mousePressedEvent() override;
    int type()const{
        return Type;
    }
private:
    QPolygonF boxPolygon;
    QPolygonF trianglePolygon;
};

