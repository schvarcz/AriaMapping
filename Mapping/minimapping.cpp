#include "minimapping.h"

MiniMapping::MiniMapping(Robot *robot,QGraphicsView *view) :
    QObject()
{
    mRobot = robot;
    mView = view;
    mScene = new QGraphicsScene();
    thread = new QThread();

    mView->setScene(mScene);
    this->moveToThread(thread);
    rangeMax = 30000;

    celRange = rangeMax/MAP_LENGTH_MINI;

    connect(thread,SIGNAL(started()),this,SLOT(keepRendering()));
    connect(thread,SIGNAL(finished()),this,SLOT(finishRendering()));
}

void MiniMapping::start()
{
    run = true;
    thread->start();
    cout << "Startado o minimap" << endl;
}

void MiniMapping::stop()
{
    run = false;
}

void MiniMapping::resetMap()
{
    cout << "Resetando o minimap" << endl;
    for(int x =0; x<MAP_LENGTH_MINI;x++)
    {
        for(int y=0; y<MAP_LENGTH_MINI;y++)
        {
            map[x][y] = 0.7;
        }
    }
    mScene->clear();
}

void MiniMapping::calculateMap()
{
    //cout << "Calculando o minimap" << endl;
    for(int x=0; x<MAP_LENGTH_MINI;x++)
    {
        for(int y=0; y<MAP_LENGTH_MINI;y++)
        {
            int angle = round(atan2(
                                  (MAP_LENGTH_MINI-1-y)*celRange,
                                  (x-MAP_LENGTH_MINI/2)*celRange
                                  )*180/M_PI);
            float distance = sqrt(
                                     pow(
                                         (x-MAP_LENGTH_MINI/2)*celRange,
                                         2
                                         )
                                     +pow(
                                         (y-MAP_LENGTH_MINI)*celRange,
                                         2
                                         )
                                     );
            if ((angle>0) && (angle<=180))
            {
                //cout << "reading " << 180-angle << "/" << sensors.size() << endl;
                float reading = (float)sensors->at(180-angle).getRange();
                //cout << "read" << endl;

                //cout << x << " " << y << " " << angle << " " << distance << " " << reading << " " << celRange << endl;

                float variacao = celRange/2;

                if(reading + variacao <= distance)
                {
                    //cout << "Área desconhecida... " << endl;
                    map[x][y] = 0.7;
                }
                else if(reading-variacao > distance)
                {
                    //cout << "Área vaga... " << endl;
                    map[x][y] = 1.0;
                }
                else if( (reading + variacao > distance) && (reading - variacao <= distance))
                {
                    //cout << "Parede... " << endl;
                    map[x][y] = 0.0;
                }

            }
            else
            {
                map[x][y] = 0.7;
            }
        }
    }
}

void MiniMapping::render()
{

    //cout << "Rendering o minimap" << endl;
    float celWidth = ((float)mView->width())/MAP_LENGTH_MINI;
    float celHeight = ((float)mView->height())/MAP_LENGTH_MINI;

    mScene = new QGraphicsScene();

    //cout << "\n\n\n\n" << "Printando visão do sensor: " << endl;
    for(int y=0; y<MAP_LENGTH_MINI;y++)
    {
        for(int x=0; x<MAP_LENGTH_MINI;x++)
        {
            //cout << map[x][MAP_LENGTH-1-y] << " ";

            if(map[x][y] < 1.0)
            {
                int value = 255*map[x][y];
                drawBox(
                            x*celWidth,
                            y*celHeight,
                            celWidth,
                            celHeight,
                            QBrush(QColor(value,value,value))
                            );
            }
        }
        //cout << endl;
    }
    emit updateScene(mScene);
}

void MiniMapping::drawBox(double x, double y, double width, double height, QBrush color)
{
    QPolygonF box;
    box << QPointF(x,y) << QPointF(x+width,y) << QPointF(x+width,y+height) << QPointF(x,y+height);
    mScene->addPolygon(box,QPen(Qt::NoPen),color);
}

void MiniMapping::keepRendering()
{
    while(run)
    {
        //resetMap();
        if(sensors != NULL)
        {
            sensors->clear();
            delete sensors;
        }
        sensors = mRobot->getLaserRanges();
        calculateMap();
        render();
        ArUtil::sleep(33);
    }
    thread->exit();
}

void MiniMapping::finishRendering()
{
}
