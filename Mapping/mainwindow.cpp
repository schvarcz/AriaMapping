#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->connectActions();

    architecture = new Architecture();
    architecture->setMiniMapGraphicsView(ui->GVSensor);

    connect(architecture->miniMap,SIGNAL(updateScene(QGraphicsScene*)),this,SLOT(updateGraphics(QGraphicsScene*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectActions()
{
    connect(ui->pbStartStop,SIGNAL(clicked()),this,SLOT(startStopRobot()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    delete architecture;
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

