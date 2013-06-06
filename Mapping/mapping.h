#ifndef MAPPING_H
#define MAPPING_H

#include <QObject>
#include <QtGui>
#include <robot.h>
#include <iostream>
#include <Aria.h>
#include <qmath.h>
#include <cellmap.h>
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFormat>

#define MAP_LENGTH_WORLD 500

using namespace std;

namespace MappingTechnique
{
    enum Techniques
    {
        DeadReckoning,
        BAYES,
        HIMM
    };
}

using namespace MappingTechnique;

class Mapping : public QObject
{
    Q_OBJECT
public:
    explicit Mapping(Robot *robot,Techniques tech = MappingTechnique::HIMM);
    void start();
    void stop();
    void render();

private:
    void resetMap();
    void calculateMapDeadReckoning();
    void calculateMapHIMM();
    void calculateMapBayes();
    void updateRoboPosition(float x,float y,float th);
    void drawBox(double xi,double yi,double xf,double yf,QColor color);
    void drawRobot();
    Robot *mRobot;
    vector<ArSensorReading> *lasers = NULL, *sonares = NULL, *sensores = NULL;
    QThread *thread;
    CellMap mapCell[MAP_LENGTH_WORLD][MAP_LENGTH_WORLD];
    bool run;
    double rangeMax, celRange;
    float xRobo,yRobo, thRobo;
    Techniques technique;



public slots:
    void keepRendering();
    void finishRendering();

};

#endif // MAPPING_H
