#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include <QObject>
#include <wallfollowing.h>
#include <robot.h>
#include <minimapping.h>
#include <QtCore>
#include <QtGui>
#include <mapping.h>

class Architecture : public QObject
{
    Q_OBJECT

public:
    explicit Architecture();
    ~Architecture();
    void startWallFollowing();
    void stopWallFollowing();
    void forward(int distance);
    void backward(int distance);
    void rotate(float degrees);
    void setMiniMapGraphicsView(QGraphicsView *view);
    QGraphicsScene* miniMapScene(){ return miniMap->mScene;}
    void setWorldMapGraphicsView(QGraphicsView *view);
    QGraphicsScene* worldMapScene(){ return worldMap->mScene; }
    MiniMapping *miniMap;
    Mapping *worldMap;

private:
    WallFollowing *wallFollowing;
    Robot *mRobot;
    QThread *thread;

public slots:

signals:
    void updateMiniMapScene(QGraphicsScene* newScene);
    void updateWorldMapScene(QGraphicsScene* newScene);

};

#endif // ARCHITECTURE_H
