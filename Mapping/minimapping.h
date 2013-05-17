#ifndef MINI_MAPPING_H
#define MINI_MAPPING_H

#include <QObject>
#include <QtGui>
#include <robot.h>
#include <iostream>
#include <Aria.h>
#include <qmath.h>

#define MAP_LENGTH_MINI 60

using namespace std;

class MiniMapping : public QObject
{
    Q_OBJECT
public:
    explicit MiniMapping(Robot *robot,QGraphicsView *view);
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
    float map[MAP_LENGTH_MINI][MAP_LENGTH_MINI];
    bool run;
    double rangeMax, celRange;



public slots:
    void keepRendering();
    void finishRendering();

signals:
    void updateScene(QGraphicsScene* newScene);
    
};

#endif // MINI_MAPPING_H
