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
            map[i][j] = 0.7;
        }
    }
    mScene->clear();
}

void Mapping::calculateMap()
{
    for(int y=0; y<MAP_LENGTH;y++)
    {
        for(int x=0; x<MAP_LENGTH;x++)
        {
            int angle = round(atan2(
                                  (MAP_LENGTH-y)*celRange,
                                  (x-MAP_LENGTH/2)*celRange
                                  )*180/M_PI);
            float distance = sqrt(
                                     pow(
                                         (x-MAP_LENGTH/2)*celRange,
                                         2
                                         )
                                     +pow(
                                         (y-MAP_LENGTH)*celRange,
                                         2
                                         )
                                     );
            float reading = (float)sensors->at(180-angle).getRange();

            //cout << x << " " << y << " " << angle << " " << distance << " " << reading << " " << celRange << endl;

            float variacao = celRange/2;

            if( (reading + variacao > distance) && (reading - variacao <= distance))
            {
                //cout << "Parede... " << endl;
                map[x][y] = 0.0;
            }
            if(reading-variacao > distance)
            {
                //cout << "Área vaga... " << endl;
                map[x][y] = 1.0;
            }
        }
    }
}

void Mapping::render()
{

    float celWidth = floor(((float)mView->width())/MAP_LENGTH);
    float celHeight = floor(((float)mView->height())/MAP_LENGTH);

    mScene = new QGraphicsScene();

    //cout << "\n\n\n\n" << "Printando visão do sensor: " << endl;
    for(int y=0; y<MAP_LENGTH;y++)
    {
        for(int x=0; x<MAP_LENGTH;x++)
        {
            //cout << map[x][MAP_LENGTH-1-y] << " ";
            int value = round(255*map[x][y]);
            drawBox(
                        round(x)*celWidth,
                        round(y)*celHeight,
                        celWidth,
                        celHeight,
                        QBrush(QColor(value,value,value))
                        );
        }
        //cout << endl;
    }
    emit updateScene(mScene);
}

void Mapping::drawBox(double x, double y, double width, double height, QBrush color)
{
    QPolygonF box;
    box << QPointF(x,y) << QPointF(x+width,y) << QPointF(x+width,y+height) << QPointF(x,y+height);
    mScene->addPolygon(box,QPen(Qt::NoPen),color);
}

void Mapping::keepRendering()
{
    while(run)
    {
        resetMap();
        mRobot->readingSensors();
        sensors = mRobot->getLaserRanges();
        calculateMap();
        render();
        ArUtil::sleep(50);
    }
    thread->exit();
}

void Mapping::finishRendering()
{
}
