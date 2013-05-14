#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <architecture.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    void connectActions();
    Ui::MainWindow *ui;
    Architecture *architecture;

private slots:
    void startStopRobot();
    void updateGraphics(QGraphicsScene* newScene);

};

#endif // MAINWINDOW_H
