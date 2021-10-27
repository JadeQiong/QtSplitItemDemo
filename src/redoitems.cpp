#include "redoitems.h"
#include<QtWidgets>
redoitems::redoitems(redoitem_type redoitemType, QGraphicsItem* item)
    :QGraphicsPolygonItem(item){
   if(redoitemType==Box){
       boxPolygon<<QPointF(0,0)<<QPointF(0,30)<<QPointF(30,30)<<QPointF(30,0)<<QPointF(0,0);
       setPolygon(boxPolygon);

   }
   else{
       trianglePolygon<<QPointF(15,0)<<QPointF(30,20)<<QPointF(0,30);

       setPolygon(trianglePolygon);
   }
    QColor color(static_cast<int>(qrand())%256,static_cast<int>(qrand())%256,static_cast<int>(qrand())%256 );
  QBrush brush(color);
    setBrush(brush);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);

}
