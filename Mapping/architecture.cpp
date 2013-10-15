#include "architecture.h"

Architecture::Architecture() : QObject()
{
    mRobot = new Robot();
    mRobot->start();

    wallFollowing = new WallFollowing(mRobot);
    mWorldMap = new Mapping(mRobot,MappingTechnique::DeadReckoning);
    mWorldMap->start();
    mCampoPotencial = new CampoPotencial(mWorldMap,500);
    mCampoPotencial->start();
    mControlRobot = new ControlRobot(mCampoPotencial,mRobot);
}

Architecture::~Architecture()
{
    wallFollowing->stop();
    mWorldMap->stop();
    mRobot->stop();
    mRobot->deleteLater();
    delete mRobot;
}

void Architecture::startWallFollowing()
{
    //mControlRobot->start();
    wallFollowing->start();
}

void Architecture::stopWallFollowing()
{
    //mControlRobot->exit();
    wallFollowing->stop();
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
