#include "architecture.h"

Architecture::Architecture() : QObject()
{
    miniMap = NULL;

    mRobot = new Robot();
    thread = new QThread();
    wallFollowing = new WallFollowing(mRobot);
    mRobot->start();
}

Architecture::~Architecture()
{
    wallFollowing->stop();
    mRobot->stop();
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

    miniMap = new Mapping(mRobot,view);
    miniMap->start();
}
