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
#include <irendermap.h>

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

class Mapping : public QObject, public IRenderMap
{
    Q_OBJECT
public:
    explicit Mapping(Robot *robot,Techniques tech = MappingTechnique::DeadReckoning);
    void start();
    void stop();
    virtual void render();
    CellMap mapCell[MAP_LENGTH_WORLD][MAP_LENGTH_WORLD];
    void drawRobot();
    float xRobo,yRobo, thRobo;

private:
    void resetMap();
    void calculateMapDeadReckoning();
    void calculateMapHIMM();
    void calculateMapBayes();
    void updateRoboPosition(float x,float y,float th);
    void drawBox(double xi,double yi,double xf,double yf,QColor color);
    Robot *mRobot;
    vector<ArSensorReading> *lasers, *sonares, *sensores;
    QThread *thread;
    bool run;
    double rangeMax, celRange;
    Techniques technique;



public slots:
    void keepRendering();
    void finishRendering();

};

#endif // MAPPING_H
