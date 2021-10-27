#pragma once
#include<QMainWindow>
class QAction;
class QMenu;
class QUndoStack;
class QUndoView;
class redoscene;

class MainWindow:public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();
private slots:
    void addBox();
private:
    void createActions();
    void createMenus();
    void createUndoView();


    QAction *addBoxAction;
    QAction *undoAction;
    QAction *redoAction;

    QMenu *editMenu;
    QMenu *itemMenu;

    redoscene *diagramScene;
    QUndoStack *undoStack;
    QUndoView *undoView;

};
