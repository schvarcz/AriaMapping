#include "architecture.h"

Architecture::Architecture() : QObject()
{
    mRobot = new Robot();
    mRobot->start();

    wallFollowing = new WallFollowing(mRobot);
    mWorldMap = new Mapping(mRobot);
}

Architecture::~Architecture()
{
    wallFollowing->stop();
    miniMap->stop();
    worldMap()->stop();
    mRobot->stop();
    mRobot->deleteLater();
    delete mRobot;
}

void Architecture::startWallFollowing()
{
    wallFollowing->start();
}

void Architecture::stopWallFollowing()
{
    wallFollowing->stop();
}

void Architecture::setMiniMapGraphicsView(QGraphicsView *view)
{
    if(miniMap != NULL)
    {
        miniMap->stop();
    }

    miniMap = new MiniMapping(mRobot,view);
    connect(miniMap,SIGNAL(updateScene(QGraphicsScene*)),this,SIGNAL(updateMiniMapScene(QGraphicsScene*)));
    miniMap->start();
}

void Architecture::setWorldMapGraphicsView()
{
    if(mWorldMap != NULL)
    {
        mWorldMap->stop();
    }

    mWorldMap = new Mapping(mRobot);
    mWorldMap->start();
}

void Architecture::forward(int distance)
{
    mRobot->move(distance);
}

void Architecture::backward(int distance)
{
    mRobot->move(-distance);
}

void Architecture::rotate(float degrees)
{
    mRobot->rotate(degrees);
}
