#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->connectActions();


    architecture.setMiniMapGraphicsView(ui->GVSensor);
    architecture.setWorldMapGraphicsView(ui->GVMap);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectActions()
{
    connect(ui->pbStartStop,SIGNAL(clicked()),this,SLOT(startStopRobot()));
    connect(&architecture,SIGNAL(updateMiniMapScene(QGraphicsScene*)),this,SLOT(updateMiniGraphics(QGraphicsScene*)));
    connect(&architecture,SIGNAL(updateWorldMapScene(QGraphicsScene*)),this,SLOT(updateWorldGraphics(QGraphicsScene*)));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
}

void MainWindow::startStopRobot()
{
    if(ui->pbStartStop->isChecked())
    {
        ui->pbStartStop->setText("Stop Robot");
        architecture->startWallFollowing();
    }
    else
    {
        ui->pbStartStop->setText("Start Robot");
        architecture->stopWallFollowing();
    }
}

void MainWindow::updateGraphics(QGraphicsScene* newScene)
{
    ui->GVSensor->setScene(newScene);
}

void MainWindow::on_pbUp_clicked()
{
    architecture->forward(100);
}

void MainWindow::on_pbDown_clicked()
{
    architecture->backward(100);
}

void MainWindow::on_pbLeft_clicked()
{
    architecture->rotate(10);
}

void MainWindow::on_pbRight_clicked()
{
    architecture->rotate(-10);
}

