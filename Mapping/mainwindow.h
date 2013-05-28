#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <architecture.h>
#include <glwidget.h>
#include <iostream>


using namespace std;

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
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);

private:
    void connectActions();
    Ui::MainWindow *ui;
    Architecture architecture;
    GLWidget *gl;
    QToolButton *up, *down, *left, *right, *playButton;

private slots:
    void updateMiniGraphics(QGraphicsScene* newScene);
    void updateWorldGraphics(QGraphicsScene* newScene);

    void on_pbLeft_clicked();
    void on_pbRight_clicked();
    void on_pbDown_clicked();
    void on_pbUp_clicked();

    void on_playbutton_clicked();
};

#endif // MAINWINDOW_H
