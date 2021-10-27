#include "commands.h"
#include"redoitems.h"
#include"QtWidgets"
commands::commands(redoitems::redoitem_type type, QGraphicsScene* graphicsScene, QUndoCommand* parent):QUndoCommand(parent)
{
static int itemcount  = 0;
myGraphicsScene = graphicsScene;
myredoitem = new redoitems(type);
initialPosition = QPointF((itemcount*15)%int(graphicsScene->width()),
                          (itemcount*15)%int(graphicsScene->height()));
myGraphicsScene->update();
itemcount++;

}

commands::~commands(){
    if(!myredoitem->scene()){
        delete myredoitem;
    }
}

void commands::redo(){
    myGraphicsScene->addItem(myredoitem);
    myredoitem->setPos(initialPosition);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}

void commands::undo(){
    myGraphicsScene->removeItem(myredoitem);
    myGraphicsScene->update();
}

