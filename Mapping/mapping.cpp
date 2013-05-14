#include "mapping.h"

Mapping::Mapping(Robot *robot,QGraphicsView *view) :
    QObject()
{
    mRobot = robot;
    mView = view;
    mScene = new QGraphicsScene();
    thread = new QThread();

    mView->setScene(mScene);
    this->moveToThread(thread);
    rangeMax = 10000;

    celRange = rangeMax/MAP_LENGTH;

    connect(thread,SIGNAL(started()),this,SLOT(keepRendering()));
    connect(thread,SIGNAL(finished()),this,SLOT(finishRendering()));
}

void Mapping::start()
{
    run = true;
    thread->start();
}

void Mapping::stop()
{
    run = false;
}

void Mapping::resetMap()
{
    for(int i =0; i<MAP_LENGTH;i++)
    {
        for(int j=0; j<MAP_LENGTH;j++)
        {
            map[i][j] = 1.0;
        }
    }
    mScene->clear();
}

void Mapping::render()
{
    for(int k =0;k<180;k++)
    {
        float range = (float)sensors->at(k).getRange()/celRange;
        int x = range*cos((180-k)*M_PI/180)+MAP_LENGTH/2;
        int y = range*sin(k*M_PI/180);
        if((x < MAP_LENGTH) && (x>=0) && (y < MAP_LENGTH) && (y >= 0))
        {
            map[x][y] = 0.0;
        }
    }

    float celWidth = ((float)mView->width())/MAP_LENGTH;
    float celHeight = ((float)mView->height())/MAP_LENGTH;

    mScene = new QGraphicsScene();

    //cout << "\n\n\n\n" << "Printando visão do sensor: " << endl;
    for(int y=0; y<MAP_LENGTH;y++)
    {
        for(int x=0; x<MAP_LENGTH;x++)
        {
            //cout << map[x][MAP_LENGTH-1-y] << " ";
            if(map[x][MAP_LENGTH-1-y] == 0.0)
            {
                drawBox(round(x)*celWidth,round(y)*celHeight,celWidth,celHeight);
            }
        }
        //cout << endl;
    }
    emit updateScene(mScene);
}

void Mapping::drawBox(double x, double y, double width, double height)
{
    QPolygonF box;
    box << QPointF(x,y) << QPointF(x+width,y) << QPointF(x+width,y+height) << QPointF(x,y+height);
    //mScene->addPolygon(box,QPen(),QBrush(QColor(0,0,0)));
    mScene->addPolygon(box);
}

void Mapping::keepRendering()
{
    while(run)
    {
        resetMap();
        mRobot->readingSensors();
        sensors = mRobot->getLaserRanges();
        render();
        ArUtil::sleep(33);
    }
    thread->exit();
}

void Mapping::finishRendering()
{
}
