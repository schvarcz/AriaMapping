#ifndef CAMPOPOTENCIAL_H
#define CAMPOPOTENCIAL_H

#include <QtCore>

#include <QColor>
#include <QGLWidget>
#include <time.h>
#include <paletacores.h>
#include <QThread>
#include <iostream>
#include <mapping.h>
#include <irendermap.h>


class CampoPotencial : public QThread, public IRenderMap
{
    Q_OBJECT
public:
    CampoPotencial(Mapping *map,int mapSize);
    void MoveRobot();
    double distance(double x1, double y1, double x2, double y2);
    void render();
    void drawRobot();
    double **mPotencial, Dmax, Dmin, Dobstacle;
    int mMapSize, celRange;
    PaletaCores paleta;
    bool time;
    struct timeval begin,end;
    Mapping *mMapping;

    float angleAndar;
public slots:
    float ComputaDistancia();

protected:
    void run();
};

#endif // CAMPOPOTENCIAL_H
