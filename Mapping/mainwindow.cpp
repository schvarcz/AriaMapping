#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->connectActions();



    gl = new GLWidget(ui->GVMap);
    ui->GVMap->setViewport(gl);

    architecture.setMiniMapGraphicsView(ui->GVSensor);
    architecture.setWorldMapGraphicsView();
    gl->setMapping(architecture.worldMap());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectActions()
{
    connect(&architecture,SIGNAL(updateMiniMapScene(QGraphicsScene*)),this,SLOT(updateMiniGraphics(QGraphicsScene*)));
    connect(&architecture,SIGNAL(updateWorldMapScene(QGraphicsScene*)),this,SLOT(updateWorldGraphics(QGraphicsScene*)));

    connect(ui->actionUp,SIGNAL(triggered()),this,SLOT(on_pbUp_clicked()));
    connect(ui->actionDown,SIGNAL(triggered()),this,SLOT(on_pbDown_clicked()));
    connect(ui->actionLeft,SIGNAL(triggered()),this,SLOT(on_pbLeft_clicked()));
    connect(ui->actionRight,SIGNAL(triggered()),this,SLOT(on_pbRight_clicked()));

    playButton = new QToolButton();
    playButton->setIcon(QIcon("icons/Play-1-Hot-icon.png"));
    playButton->setCheckable(true);
    ui->mainToolBar->addWidget(playButton);
    connect(playButton,SIGNAL(clicked()),this,SLOT(on_playbutton_clicked()));

    up = new QToolButton();
    up->setIcon(QIcon("icons/arrow-up-icon.png"));
    ui->mainToolBar->addWidget(up);
    connect(up,SIGNAL(clicked()),this,SLOT(on_pbUp_clicked()));

    down = new QToolButton();
    down->setIcon(QIcon("icons/arrow-down-icon.png"));
    ui->mainToolBar->addWidget(down);
    connect(down,SIGNAL(clicked()),this,SLOT(on_pbDown_clicked()));

    left = new QToolButton();
    left->setIcon(QIcon("icons/arrow-left-icon.png"));
    ui->mainToolBar->addWidget(left);
    connect(left,SIGNAL(clicked()),this,SLOT(on_pbLeft_clicked()));

    right = new QToolButton();
    right->setIcon(QIcon("icons/arrow-right-icon.png"));
    ui->mainToolBar->addWidget(right);
    connect(right,SIGNAL(clicked()),this,SLOT(on_pbRight_clicked()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    gl->resize(ui->GVMap->width(),ui->GVMap->height());
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    gl->resize(ui->GVMap->width(),ui->GVMap->height());
}


void MainWindow::updateMiniGraphics(QGraphicsScene* newScene)
{
    QGraphicsScene* oldScene = ui->GVSensor->scene();
    ui->GVSensor->setScene(newScene);
    ui->GVSensor->fitInView(0,0,newScene->width(),newScene->height());
    oldScene->clear();
    delete oldScene;
}

void MainWindow::updateWorldGraphics(QGraphicsScene* newScene)
{
    QGraphicsScene* oldScene = ui->GVMap->scene();
    ui->GVMap->setScene(newScene);
    oldScene->clear();
    delete oldScene;
}

void MainWindow::on_pbUp_clicked()
{
    architecture.forward(100);
}

void MainWindow::on_pbDown_clicked()
{
    architecture.backward(100);
}

void MainWindow::on_pbLeft_clicked()
{
    architecture.rotate(10);
}

void MainWindow::on_pbRight_clicked()
{
    architecture.rotate(-10);
}

void MainWindow::on_playbutton_clicked()
{
    if(playButton->isChecked())
    {
        playButton->setIcon(QIcon("icons/Pause-Normal-Red-icon.png"));
        architecture.startWallFollowing();
    }
    else
    {
        playButton->setIcon(QIcon("icons/Play-1-Hot-icon.png"));
        architecture.stopWallFollowing();
    }
}

