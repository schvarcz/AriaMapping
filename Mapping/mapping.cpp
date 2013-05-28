#include "mapping.h"

Mapping::Mapping(Robot *robot) :
    QObject()
{
    mRobot = robot;
    thread = new QThread();

    this->moveToThread(thread);
    rangeMax = 30000;

    celRange = 100000/MAP_LENGTH_WORLD;

    celWidth = 1000.0/MAP_LENGTH_WORLD;
    celHeight = 700.0/MAP_LENGTH_WORLD;


    shiftX = 1000.0/2;
    shiftY = 700.0/2;

    connect(thread,SIGNAL(started()),this,SLOT(keepRendering()));
    connect(thread,SIGNAL(finished()),this,SLOT(finishRendering()));
}

void Mapping::start()
{
    run = true;
    thread->start();
    cout << "Startado o minimap" << endl;
}

void Mapping::stop()
{
    run = false;
}

void Mapping::resetMap()
{
    cout << "Resetando o map" << endl;
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
  ArSensorReading ar = sensors->at(0);
  cout << "x: " << ar.getX() << " y: " << ar.getY() << endl;
  cout << "Taken x: " << ar.getXTaken() << " y: " << ar.getYTaken() << " th: " << ar.getThTaken() << endl;
  updateRoboPosition(ar.getXTaken()/celRange,ar.getYTaken()/celRange);

  for(int x=0; x<MAP_LENGTH_WORLD;x++)
  {
      for(int y=0; y<MAP_LENGTH_WORLD;y++)
      {
          int angle = round(atan2(
                                ar.getYTaken()-y*celRange,
                                x*celRange-ar.getXTaken()
                                )*180/M_PI);
          float distance = sqrt(
                                   pow(
                                       x*celRange-ar.getXTaken(),
                                       2
                                       )
                                   +pow(
                                       y*celRange-ar.getYTaken(),
                                       2
                                       )
                                   );
          if ((angle>0) && (angle<=180))
          {
              //cout << "reading " << 180-angle << "/" << sensors.size() << endl;
              float reading = (float)sensors->at(180-angle).getRange();
              ArSensorReading ar = sensors->at(180-angle);
              cout << "x: " << ar.getX() << " y: " << ar.getY() << endl;
              cout << "Taken x: " << ar.getXTaken() << " y: " << ar.getYTaken() << " th: " << ar.getThTaken() << endl;
              //cout << "read" << endl;

              //cout << x << " " << y << " " << angle << " " << distance << " " << reading << " " << celRange << endl;

              float variacao = celRange/2;

              if(reading + variacao <= distance)
              {
                  //cout << "Área desconhecida... " << endl;
                  mapCell[x][y] = 0.7;
              }
              else if(reading-variacao > distance)
              {
                  //cout << "Área vaga... " << endl;
                  mapCell[x][y] = 1.0;
              }
              else if( (reading + variacao > distance) && (reading - variacao <= distance))
              {
                  //cout << "Parede... " << endl;
                  mapCell[x][y] = 0.0;
              }

          }
          else
          {
              mapCell[x][y] = 0.7;
          }
      }
  }

  /*
  float celWidth = 1000.0/MAP_LENGTH_WORLD;
  float celHeight = 700.0/MAP_LENGTH_WORLD;
  QRect ret(QPoint(celWidth*MAP_LENGTH_WORLD/2,celHeight*MAP_LENGTH_WORLD/2),QPoint(celWidth*MAP_LENGTH_WORLD,celHeight*MAP_LENGTH_WORLD));
  QList<QGraphicsView*> views = mScene->views();
  for(int i=0;i<views.size();i++)
  {
      views.at(i)->viewport()->update(ret);
  }*/
}

void Mapping::updateRoboPosition(float x, float y)
{
    /*
    QPolygonF box;
    box << QPointF(x+shiftX,y+shiftY)
        << QPointF(x+shiftX+celWidth*5,y+shiftY)
        << QPointF(x+shiftX+celWidth*5,y+celHeight*5+shiftY)
        << QPointF(x+shiftX,y+celHeight*5+shiftY);
    roboPoly->setPolygon(box);*/
}
void Mapping::render()
{

    cout << "Rendering o map" << endl;

    glBegin(GL_POINTS);

    //  //cout << "\n\n\n\n" << "Printando visão do sensor: " << endl;
    for(int y=0; y<MAP_LENGTH_WORLD;y++)
    {
        for(int x=0; x<MAP_LENGTH_WORLD;x++)
        {
            //cout << map[x][MAP_LENGTH-1-y] << " ";
            if(mapCell[x][y] < 1.0)
            {
                //int value = mapCell[x][y]*255;
                drawBox(
                            x*celWidth,
                            y*celHeight,
                            celWidth,
                            celHeight
                            );
            }
        }
        //cout << endl;
    }
    glEnd();

    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_QUADS);


    glVertex2i(-10,5);
    glVertex2i(10,5);
    glVertex2i(10,-5);
    glVertex2i(-10,-5);

    glVertex2i(-5,12);
    glVertex2i(5,12);
    glVertex2i(5,-12);
    glVertex2i(-5,-12);

    glEnd();

    glBegin(GL_TRIANGLES);

    glVertex2i(-5,-12);
    glVertex2i(-5,-5);
    glVertex2i(-10,-5);

    glVertex2i(5,12);
    glVertex2i(5,5);
    glVertex2i(10,5);

    glVertex2i(-5,12);
    glVertex2i(-5,5);
    glVertex2i(-10,5);


    glVertex2i(5,-12);
    glVertex2i(5,-5);
    glVertex2i(10,-5);

    glEnd();

    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);


    glVertex2i(-5,-12);
    glVertex2i(5,-12);
    glVertex2i(5,0);
    glVertex2i(-5,0);

    glColor3f(0.0f,0.0f,1.0f);
    glVertex2i(-5,10);
    glVertex2i(5,10);
    glVertex2i(5,5);
    glVertex2i(-5,5);

    glEnd();
}

void Mapping::drawBox(double x, double y, double width, double height)
{
    //glColor3f(color.redF()/255.0f,color.greenF()/255.0f,color.blueF()/255.0f);
    glColor3f(1.0,1.0,0.0);

    glVertex2f(x,y);
    glVertex2f(x,y);
    glVertex2f(x+width,y);
    glVertex2f(x+width,y+height);
    glVertex2f(x,y+height);
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
}

void Mapping::finishRendering()
{
}
