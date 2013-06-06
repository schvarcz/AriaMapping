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

    connect(ui->actionUp,SIGNAL(triggered()),this,SLOT(roboUp()));
    connect(ui->actionDown,SIGNAL(triggered()),this,SLOT(roboDown()));
    connect(ui->actionLeft,SIGNAL(triggered()),this,SLOT(roboLeft()));
    connect(ui->actionRight,SIGNAL(triggered()),this,SLOT(roboRight()));

    playButton = new QToolButton();
    playButton->setIcon(QIcon("icons/Play-1-Hot-icon.png"));
    playButton->setCheckable(true);
    ui->mainToolBar->addWidget(playButton);
    connect(playButton,SIGNAL(toggled(bool)),this,SLOT(startStopWallFollowing()));

    up = new QToolButton();
    up->setIcon(QIcon("icons/arrow-up-icon.png"));
    ui->mainToolBar->addWidget(up);
    connect(up,SIGNAL(clicked()),this,SLOT(roboUp()));

    down = new QToolButton();
    down->setIcon(QIcon("icons/arrow-down-icon.png"));
    ui->mainToolBar->addWidget(down);
    connect(down,SIGNAL(clicked()),this,SLOT(roboDown()));

    left = new QToolButton();
    left->setIcon(QIcon("icons/arrow-left-icon.png"));
    ui->mainToolBar->addWidget(left);
    connect(left,SIGNAL(clicked()),this,SLOT(roboLeft()));

    right = new QToolButton();
    right->setIcon(QIcon("icons/arrow-right-icon.png"));
    ui->mainToolBar->addWidget(right);
    connect(right,SIGNAL(clicked()),this,SLOT(roboRight()));

    connect(ui->actionStart_Stop_Wall_Following,SIGNAL(toggled(bool)),playButton,SLOT(setChecked(bool)));
    connect(playButton,SIGNAL(toggled(bool)),ui->actionStart_Stop_Wall_Following,SLOT(setChecked(bool)));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->gl->stopRendering();
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

void MainWindow::roboUp()
{
    architecture.forward(200);
}

void MainWindow::roboDown()
{
    architecture.backward(200);
}

void MainWindow::roboLeft()
{
    architecture.rotate(15);
}

void MainWindow::roboRight()
{
    architecture.rotate(-15);
}

void MainWindow::startStopWallFollowing()
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

