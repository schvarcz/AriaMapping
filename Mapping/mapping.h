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

class Mapping : public QObject
{
    Q_OBJECT
public:
    explicit Mapping(Robot *robot);
    void start();
    void stop();
    void render();

private:
    void resetMap();
    void calculateMap();
    void updateRoboPosition(float x,float y);
    void drawBox(double xi,double yi,double xf,double yf);
    Robot *mRobot;
    vector<ArSensorReading> *sensors = NULL;
    QThread *thread;
    CellMap mapCell[MAP_LENGTH_WORLD][MAP_LENGTH_WORLD];
    QGraphicsPolygonItem* roboPoly;
    bool run;
    double rangeMax, celRange;
    float celWidth, celHeight, shiftX, shiftY;



public slots:
    void keepRendering();
    void finishRendering();

signals:
    void updateScreen();

};

#endif // MAPPING_H
