#include "architecture.h"

Architecture::Architecture() : QObject()
{
    miniMap = NULL;
    worldMap = NULL;

    mRobot = new Robot();
    mRobot->start();

    thread = new QThread();
    wallFollowing = new WallFollowing(mRobot);
    mRobot->start();
}

Architecture::~Architecture()
{
    wallFollowing->stop();
    miniMap->stop();
    mRobot->stop();
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

void Architecture::setWorldMapGraphicsView(QGraphicsView *view)
{
    if(worldMap != NULL)
    {
        worldMap->stop();
    }

    worldMap = new Mapping(mRobot,view);
    connect(worldMap,SIGNAL(updateScene(QGraphicsScene*)),this,SIGNAL(updateWorldMapScene(QGraphicsScene*)));
    //worldMap->start();
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
