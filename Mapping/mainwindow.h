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
    
protected:
    void closeEvent(QCloseEvent *event);

private:
    void connectActions();
    Ui::MainWindow *ui;
    Architecture *architecture;

private slots:
    void startStopRobot();
    void updateGraphics(QGraphicsScene* newScene);

    void on_pbLeft_clicked();
    void on_pbRight_clicked();
    void on_pbDown_clicked();
    void on_pbUp_clicked();
};

#endif // MAINWINDOW_H
