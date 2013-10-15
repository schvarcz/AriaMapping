#include "mapping.h"

Mapping::Mapping(Robot *robot,Techniques tech) :
    QObject(),
    lasers(NULL),
    sonares(NULL),
    sensores(NULL)
{
    mRobot = robot;
    this->technique = tech;
    thread = new QThread();

    this->moveToThread(thread);
    rangeMax = 5000;

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

void Mapping::calculateMapDeadReckoning()
{
    cout << "Calculando o mapa pelo metodo de Dead Reckoning" << endl;

    float variacao = 100;//10 centimetros

    for(int x=0; x<MAP_LENGTH_WORLD;x++)
    {
        for(int y=0; y<MAP_LENGTH_WORLD;y++)
        {
            int angle = round(atan2(
                                  ((float)MAP_LENGTH_WORLD/2 - y + 0.5 )*celRange-yRobo,
                                  ((float)x + 0.5 - MAP_LENGTH_WORLD/2)*celRange-xRobo
                                  )*180/M_PI)+90-thRobo;
            float distance = sqrt(
                        pow(
                            (x + 0.5 - MAP_LENGTH_WORLD/2)*celRange - xRobo,
                            2
                            )
                        +pow(
                            (MAP_LENGTH_WORLD/2 - y + 0.5 )*celRange - yRobo,
                            2
                            )
                        );

            if(angle<0)
                angle += 360;
            if(angle>360)
                angle %= 360;

            if ((angle>0) && (angle<=180))
            {
                float reading = (float)lasers->at(180-angle).getRange();

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

void Mapping::calculateMapBayes()
{
    cout << "Calculando o mapa pelo metodo de Bayes" << endl;

    float variacao = 100;//10 centimetros
    float pMax = 0.90;
    double aberturaSonar = 20.0;

    for(int x=0; x<MAP_LENGTH_WORLD;x++)
    {
        for(int y=0; y<MAP_LENGTH_WORLD;y++)
        {
            double angle = round(atan2(
                                  ((float)MAP_LENGTH_WORLD/2 - y + 0.5 )*celRange-yRobo,
                                  ((float)x + 0.5 - MAP_LENGTH_WORLD/2)*celRange-xRobo
                                  )*180/M_PI)-thRobo;
            float distance = sqrt(
                        pow(
                            (x + 0.5 - MAP_LENGTH_WORLD/2)*celRange - xRobo,
                            2
                            )
                        +pow(
                            (MAP_LENGTH_WORLD/2 - y + 0.5 )*celRange - yRobo,
                            2
                            )
                        );

            if(angle < -180)
                angle += 360;
            if(angle > 180)
                angle -= 360;

            for(int i =0;i<sonares->size();i++)
            {

                float angleSensor = sonares->at(i).getSensorTh();

                if(fabs(angle-angleSensor) <= aberturaSonar/2)
                {
                    float reading = sonares->at(i).getRange();
                    double pOcupada, pVazia;
                    pOcupada = pVazia = 0.5;

                    if(reading > rangeMax)
                    {
                        reading = rangeMax;
                    }
                    if(reading + variacao < distance)
                    {
                        //cout << "Área desconhecida... " << endl;
                    }
                    else if(reading == rangeMax)
                    {
                        pVazia = pMax;
                        pOcupada = 1.0-pVazia;
                    }
                    else if(reading - variacao > distance)
                    {
                        //cout << "Área vaga... " << endl;
                        pVazia = 0.5*((2*rangeMax-distance)/(2*rangeMax) + (aberturaSonar-fabs(angle-angleSensor))/aberturaSonar);
                        pVazia = max(pVazia, 1.0-pMax);
                        pOcupada = 1.0 - pVazia;

                    }
                    else if((reading - variacao <= distance) && (reading + variacao >= distance))
                    {
                        //cout << "Parede... " << endl;
                        pOcupada = 0.5*pMax*((2*rangeMax-distance)/(2*rangeMax) + (aberturaSonar-fabs(angle-angleSensor))/aberturaSonar);
                        //pVazia = max(pOcupada, 1.0-pMax);
                        pVazia = 1.0 - pOcupada;

                    }
                    mapCell[x][y].setProbabilidadeOcupada(
                                pOcupada*mapCell[x][y].probabilidadeOcupada()
                                /
                                (pOcupada*mapCell[x][y].probabilidadeOcupada() + pVazia*mapCell[x][y].probabilidadeVazia())
                                );

                    if(mapCell[x][y].probabilidadeVazia() == 0.0)
                        cout << "ZERO!!!!" << endl;
                    break;
                }
            }
        }
    }

}

void Mapping::calculateMapHIMM()
{
    cout << "Calculando o mapa pelo metodo de HIMM" << endl;

    for (int i =0;i<sonares->size();i++)
    {
        //cout << sonares->at(i).getSensorTh() << ": " << sonares->at(i).getRange() << endl;

        float range = sonares->at(i).getRange(), angle = sonares->at(i).getSensorTh()+thRobo;

        if(range > rangeMax)
            range = rangeMax;

        int x = floor(range*cos(angle*M_PI/180)/celRange), y = floor(range*sin(angle*M_PI/180)/celRange),
                xRobo = floor(this->xRobo/celRange), yRobo = floor(this->yRobo/celRange);


        if(range != rangeMax)
            mapCell[xRobo+x+MAP_LENGTH_WORLD/2][MAP_LENGTH_WORLD/2 - (yRobo+y)].addHimmProbability();

        double dx = x-xRobo, dy = y-yRobo;
        float m = max(fabs(dx),fabs(dy));
        double xStep = x/m, yStep = y/m;

        for(int n=0;n<m;n++)
        {

            int nX = xRobo + n*xStep + MAP_LENGTH_WORLD/2;
            int nY = MAP_LENGTH_WORLD/2 - (yRobo + n*yStep);
            if((nX != xRobo+x+MAP_LENGTH_WORLD/2) && (nY != MAP_LENGTH_WORLD/2 - (yRobo+y)))
                mapCell[nX][nY].subHimmProbability();

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
            double value;

            switch (technique) {
            case MappingTechnique::BAYES:
                value = 1.0 - mapCell[x][y].probabilidadeOcupada();
                //value = 1.0 - mapCell[x][y].isBayesProbability();
                break;
            case MappingTechnique::HIMM:
                value = mapCell[x][y].himmProbability();
                break;
            case MappingTechnique::DeadReckoning:
            default:
                value = mapCell[x][y].cellValue();
                break;
            }
            //qDebug() << value << endl;
            if(value != 0.7 && value >= 0 && value != 0.5)
            {
                value *= 255;
                drawBox(
                            (x-MAP_LENGTH_WORLD/2)*celRange-1,
                            (MAP_LENGTH_WORLD/2 - y)*celRange-1,
                            celRange,
                            celRange,
                            QColor(value,value,value)
                            );
            }
        }
    }
    glEnd();

    drawRobot();
}

void Mapping::drawBox(double x, double y, double width, double height,QColor color)
{
    glColor3f(color.redF(),color.greenF(),color.blueF());

    glVertex3f(x,y,0.0f);
    glVertex3f(x+width,y,0.0f);
    glVertex3f(x+width,y+height,0.0f);
    glVertex3f(x,y+height,0.0f);
}

void Mapping::drawRobot()
{
    glTranslated(xRobo,yRobo,0.0);
    glRotated(thRobo-90,0.0,0.0,1.0);

    glColor3f(1.0f,0.0f,0.0f);


    int fator = 20;

    //Draw red part (body)
    glBegin(GL_QUADS);

    glVertex3i(-10*fator,5*fator,15000.0f);
    glVertex3i(10*fator,5*fator,15000.0f);
    glVertex3i(10*fator,-5*fator,15000.0f);
    glVertex3i(-10*fator,-5*fator,15000.0f);

    glVertex3i(-5*fator,12*fator,15000.0f);
    glVertex3i(5*fator,12*fator,15000.0f);
    glVertex3i(5*fator,-12*fator,15000.0f);
    glVertex3i(-5*fator,-12*fator,15000.0f);

    glEnd();

    glBegin(GL_TRIANGLES);

    glVertex3i(-5*fator,-12*fator,15000.0f);
    glVertex3i(-5*fator,-5*fator,15000.0f);
    glVertex3i(-10*fator,-5*fator,15000.0f);

    glVertex3i(5*fator,12*fator,15000.0f);
    glVertex3i(5*fator,5*fator,15000.0f);
    glVertex3i(10*fator,5*fator,15000.0f);

    glVertex3i(-5*fator,12*fator,15000.0f);
    glVertex3i(-5*fator,5*fator,15000.0f);
    glVertex3i(-10*fator,5*fator,15000.0f);


    glVertex3i(5*fator,-12*fator,15000.0f);
    glVertex3i(5*fator,-5*fator,15000.0f);
    glVertex3i(10*fator,-5*fator,15000.0f);

    glEnd();

    //Draw black box (deck)
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);

    glVertex3i(-5*fator,-12*fator,15000.0f+1.0);
    glVertex3i(5*fator,-12*fator,15000.0f+1.0);
    glVertex3i(5*fator,0*fator,15000.0f+1.0);
    glVertex3i(-5*fator,0*fator,15000.0f+1.0);

    //Draw blue box (laser)
    glColor3f(0.0f,0.0f,1.0f);
    glVertex3i(-5*fator,10*fator,15000.0f+1.0);
    glVertex3i(5*fator,10*fator,15000.0f+1.0);
    glVertex3i(5*fator,5*fator,15000.0f+1.0);
    glVertex3i(-5*fator,5*fator,15000.0f+1.0);

    glEnd();


    glRotated(90-thRobo,0.0,0.0,1.0);
    glTranslated(-xRobo,-yRobo,0.0);
}

void Mapping::keepRendering()
{
    resetMap();
    while(run)
    {
        if(lasers != NULL)
        {
            lasers->clear();
            delete lasers;
        }
        mRobot->readingSensors();
        lasers = mRobot->getLaserRanges();
        sonares = mRobot->getSonarRanges();
        sensores = sonares;

        ArSensorReading ar = sensores->at(0); //Apenas para pegar as informações de posição do robo no momento da tomada de informações.
        updateRoboPosition(ar.getXTaken(),ar.getYTaken(),ar.getThTaken());
        switch(this->technique)
        {
        case MappingTechnique::DeadReckoning:
            calculateMapDeadReckoning();
            break;
        case MappingTechnique::BAYES:
            calculateMapBayes();
            break;
        case MappingTechnique::HIMM:
            calculateMapHIMM();
            break;
        }
        ArUtil::sleep(33);
    }
    thread->exit();
    thread->wait();
}

void Mapping::finishRendering()
{
}
