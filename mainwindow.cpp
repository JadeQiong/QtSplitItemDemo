//mainwindow.cpp

#include <QtWidgets>
#include "mainwindow.h"
#include "redoitems.h"
#include "redoscene.h"
#include "commands.h"
MainWindow::MainWindow()
{
    undoStack = new QUndoStack(this);

    createActions();
    createMenus();
    createUndoView();

    diagramScene = new redoscene();

    diagramScene->setSceneRect(QRect(0, 0, 500, 500));
    QGraphicsView *view = new QGraphicsView(diagramScene);
    setCentralWidget(view);

    setWindowTitle("UnRedo");
    resize(700,500);
}

MainWindow::~MainWindow()
{}

void MainWindow::createActions()
{
    addBoxAction = new QAction("Add Box",this);
    connect(addBoxAction,SIGNAL(triggered()),this,SLOT(addBox()));

    undoAction = undoStack->createUndoAction(this,"Undo");
    redoAction = undoStack->createRedoAction(this,"Redo");
}

void MainWindow::createMenus()
{
    editMenu = menuBar()->addMenu("Edit");
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);

    itemMenu = menuBar()->addMenu("Item");
    itemMenu->addAction(addBoxAction);
}

void MainWindow::createUndoView()
{
    undoView = new QUndoView(undoStack);
    undoView->setWindowTitle("Command List");
    undoView->show();
    undoView->setAttribute(Qt::WA_QuitOnClose, false);
}

void MainWindow::addBox()
{
    QUndoCommand *addCommand = new commands(redoitems::Box, diagramScene);
    undoStack->push(addCommand);
}
