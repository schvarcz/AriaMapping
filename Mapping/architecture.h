#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include <QObject>
#include <wallfollowing.h>
#include <robot.h>
#include <mapping.h>
#include <QtCore>
#include <QtGui>

class Architecture : public QObject
{
    Q_OBJECT

public:
    explicit Architecture();
    ~Architecture();
    void startWallFollowing();
    void stopWallFollowing();
    void setMiniMapGraphicsView(QGraphicsView *view);
    QGraphicsScene* miniMapScene(){ return miniMap->mScene;}
    Mapping *miniMap;

private:
    WallFollowing *wallFollowing;
    Robot *mRobot;
    QThread *thread;

public slots:

signals:

};

#endif // ARCHITECTURE_H
