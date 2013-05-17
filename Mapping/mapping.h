#ifndef MAPPING_H
#define MAPPING_H

#include <QObject>
#include <QtGui>
#include <robot.h>
#include <iostream>
#include <Aria.h>
#include <qmath.h>

#define MAP_LENGTH_WORLD 60

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
    void drawBox(double xi,double yi,double xf,double yf, QBrush color);
    Robot *mRobot;
    QGraphicsView *mView;
    vector<ArSensorReading> *sensors = NULL;
    QThread *thread;
    float map[MAP_LENGTH_WORLD][MAP_LENGTH_WORLD];
    bool run;
    double rangeMax, celRange;



public slots:
    void keepRendering();
    void finishRendering();

signals:
    void updateScene(QGraphicsScene* newScene);

};

#endif // MAPPING_H
