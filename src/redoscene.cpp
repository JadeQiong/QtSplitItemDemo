#include "redoscene.h"
#include"redoitems.h"
#include<QtWidgets>
redoscene::redoscene(QObject* parent):QGraphicsScene(parent)
{
    movingItem = 0;
}
void redoscene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    QPointF mousePos = event->buttonDownScenePos(Qt::LeftButton);
    const QList<QGraphicsItem*> itemList = items(mousePos);
    movingItem = itemList.isEmpty()?0:itemList.first();
    if(movingItem!=0 && event->button() == Qt::LeftButton){
        oldPos = movingItem->pos();
    }
    clearSelection();
    QGraphicsScene::mousePressEvent(event);
}
void redoscene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton && movingItem){
        if(oldPos!=movingItem->pos()){
            emit itemMoved(qgraphicsitem_cast<redoitems*> (movingItem),oldPos);
            movingItem = 0;
        }
    }
    QGraphicsScene::mouseReleaseEvent(event);
}
