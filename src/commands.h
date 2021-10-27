#pragma once
#include<QUndoCommand>
#include"redoitems.h"

class commands:public QUndoCommand
{
public:
    commands(redoitems::redoitem_type addType, QGraphicsScene* graphicsScene, QUndoCommand* parent=0);
    ~commands();
    void undo();
    void redo();
private:
    redoitems* myredoitem;
    QGraphicsScene * myGraphicsScene;
    QPointF initialPosition;
};

QString createCommandString(redoitems* item, const QPointF &point);


