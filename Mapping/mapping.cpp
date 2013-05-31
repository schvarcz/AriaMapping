#include "mapping.h"

Mapping::Mapping(Robot *robot) :
    QObject()
{
    mRobot = robot;
    thread = new QThread();

    this->moveToThread(thread);
    rangeMax = 3000;

    celRange = 2*30000/MAP_LENGTH_WORLD;

    connect(thread,SIGNAL(started()),this,SLOT(keepRendering()));
    connect(thread,SIGNAL(finished()),this,SLOT(finishRendering()));
}

void Mapping::start()
{
    qDebug() << "Start mapping" << endl;
    run = true;
    thread->start();
}

void Mapping::stop()
{
    qDebug() << "Stop mapping" << endl;
    run = false;
}

void Mapping::resetMap()
{
    qDebug() << "Resetando o mapping" << endl;
    for(int x =0; x<MAP_LENGTH_WORLD;x++)
    {
        for(int y=0; y<MAP_LENGTH_WORLD;y++)
        {
            mapCell[x][y] = 0.7;
        }
    }
}

void Mapping::calculateMap()
{
    ArSensorReading ar = sensors->at(0); //Apenas para pegar as informações de posição do robo no momento da tomada de informações.

    updateRoboPosition(ar.getXTaken(),ar.getYTaken(),ar.getThTaken());

    cout << "Calculando o mapa" << endl;

    float variacao = 100;//10 centimetros

    for(int x=0; x<MAP_LENGTH_WORLD;x++)
    {
        for(int y=0; y<MAP_LENGTH_WORLD;y++)
        {
            int angle = round(atan2(
                                  ((float)y + 0.5- MAP_LENGTH_WORLD/2)*celRange-yRobo,
                                  ((float)x + 0.5 - MAP_LENGTH_WORLD/2)*celRange-xRobo
                                  )*180/M_PI)+90-thRobo;
            float distance = sqrt(
                        pow(
                            (x + 0.5 - MAP_LENGTH_WORLD/2)*celRange - xRobo,
                            2
                            )
                        +pow(
                            (y + 0.5 - MAP_LENGTH_WORLD/2 )*celRange - yRobo,
                            2
                            )
                        );

            if ((angle>0) && (angle<=180))
            {
                float reading = (float)sensors->at(180-angle).getRange();

                if(reading > rangeMax)
                    reading = rangeMax;

                //cout << x << " " << y << " " << angle << " " << distance << " " << reading << " " << celRange << endl;

                if(reading + variacao < distance)
                {
                    //cout << "Área desconhecida... " << endl;
                    //mapCell[x][y] = 0.7;
                }
                else if(reading - variacao > distance)
                {
                    //cout << "Área vaga... " << endl;
                    mapCell[x][y] = 1.0;
                    //cout << xRobo << " " << yRobo << " " << -(MAP_LENGTH_WORLD/2 - y)*celRange << " " << distance << endl;
                }
                else if((reading != rangeMax) && (reading - variacao <= distance) && (reading + variacao >= distance))
                {
                    //cout << "Parede... " << endl;
                    mapCell[x][y] = 0.0;
                }

            }
            else
            {
                //mapCell[x][y] = 0.7;
            }
        }
    }
}

void Mapping::updateRoboPosition(float x, float y, float th)
{
    //cout << "X: " << x << "  Y: " << y << endl;
    xRobo = x;
    yRobo = y;
    thRobo = th;
}
void Mapping::render()
{

    cout << "Rendering o map" << endl;

    glBegin(GL_QUADS);

    //  //cout << "\n\n\n\n" << "Printando visão do sensor: " << endl;
    for(int y=0; y<MAP_LENGTH_WORLD;y++)
    {
        for(int x=0; x<MAP_LENGTH_WORLD;x++)
        {
            //cout << map[x][MAP_LENGTH-1-y] << " ";
            if(mapCell[x][y] != 0.7)
            {
                int value = mapCell[x][y].cellValue()*255;
                drawBox(
                            (x-MAP_LENGTH_WORLD/2)*celRange-1,
                            (y-MAP_LENGTH_WORLD/2)*celRange-1,
                            celRange,
                            celRange,
                            QColor(value,value,value)
                            );
            }
        }
        //cout << endl;
    }
    glEnd();

    drawRobot();
}

void Mapping::drawBox(double x, double y, double width, double height,QColor color)
{
    //glColor3f(color.redF()/255.0f,color.greenF()/255.0f,color.blueF()/255.0f);
    glColor3f(color.redF(),color.greenF(),color.blueF());
    //glColor3f(0.7,0.7,0.7);

    glVertex2f(x,y);
    glVertex2f(x+width,y);
    glVertex2f(x+width,y+height);
    glVertex2f(x,y+height);
}

void Mapping::drawRobot()
{
    glTranslated(xRobo,yRobo,0.0);
    glRotated(thRobo-90,0.0,0.0,1.0);

    glColor3f(1.0f,0.0f,0.0f);

    glBegin(GL_QUADS);

    int fator = 20;
    glVertex2i(-10*fator,5*fator);
    glVertex2i(10*fator,5*fator);
    glVertex2i(10*fator,-5*fator);
    glVertex2i(-10*fator,-5*fator);

    glVertex2i(-5*fator,12*fator);
    glVertex2i(5*fator,12*fator);
    glVertex2i(5*fator,-12*fator);
    glVertex2i(-5*fator,-12*fator);

    glEnd();

    glBegin(GL_TRIANGLES);

    glVertex2i(-5*fator,-12*fator);
    glVertex2i(-5*fator,-5*fator);
    glVertex2i(-10*fator,-5*fator);

    glVertex2i(5*fator,12*fator);
    glVertex2i(5*fator,5*fator);
    glVertex2i(10*fator,5*fator);

    glVertex2i(-5*fator,12*fator);
    glVertex2i(-5*fator,5*fator);
    glVertex2i(-10*fator,5*fator);


    glVertex2i(5*fator,-12*fator);
    glVertex2i(5*fator,-5*fator);
    glVertex2i(10*fator,-5*fator);

    glEnd();

    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);


    glVertex2i(-5*fator,-12*fator);
    glVertex2i(5*fator,-12*fator);
    glVertex2i(5*fator,0*fator);
    glVertex2i(-5*fator,0*fator);

    glColor3f(0.0f,0.0f,1.0f);
    glVertex2i(-5*fator,10*fator);
    glVertex2i(5*fator,10*fator);
    glVertex2i(5*fator,5*fator);
    glVertex2i(-5*fator,5*fator);

    glEnd();
}

void Mapping::keepRendering()
{
    resetMap();
    while(run)
    {
        if(sensors != NULL)
        {
            sensors->clear();
            delete sensors;
        }
        mRobot->readingSensors();
        sensors = mRobot->getLaserRanges();
        calculateMap();
        ArUtil::sleep(33);
    }
    thread->exit();
    thread->wait();
}

void Mapping::finishRendering()
{
}
