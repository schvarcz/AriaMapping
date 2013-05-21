#ifndef MAPPING_H
#define MAPPING_H

#include <QObject>
#include <QtGui>
#include <robot.h>
#include <iostream>
#include <Aria.h>
#include <qmath.h>
#include <cellmap.h>

#define MAP_LENGTH_WORLD 1000

using namespace std;

class Mapping : public QObject
{
    Q_OBJECT
public:
    explicit Mapping(Robot *robot,QGraphicsView *view);
    void start();
    void stop();
    QGraphicsScene *mScene;

private:
    void resetMap();
    void calculateMap();
    void render();
    void updateRoboPosition(float x,float y);
    QGraphicsPolygonItem* drawBox(double xi,double yi,double xf,double yf, QBrush color);
    Robot *mRobot;
    QGraphicsView *mView;
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
    void updateScene(QGraphicsScene* newScene);

};

#endif // MAPPING_H
