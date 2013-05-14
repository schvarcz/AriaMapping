#include "robot.h"

Robot::Robot(const char * name, bool ignored,
             bool doSigHandle,
             bool normalInit, bool addAriaExitCallback):
    ArRobot(name, ignored, doSigHandle, normalInit, addAriaExitCallback),
    QObject()
{
    int argc = 0;
    char *argv = new char[0];
    parser = new ArArgumentParser(&argc,&argv);
    robotConnector = new ArRobotConnector(parser,this);
    laserConnector = new ArLaserConnector(parser,this,robotConnector);
}

bool Robot::start()
{
    Aria::init();
    //parser->addDefaultArgument("-rh 192.168.1.11 -remoteLaserTcpPort 10002");
    parser->addDefaultArgument("");

    ArLog::log(ArLog::Normal,"Aria funcionando %s",parser->getArgv());
    this->addRangeDevice(&sick);

    if(!robotConnector->connectRobot())
    {
        ArLog::log(ArLog::Terse,"Ops... falha ao conectar ao servidor do robo.");
        return false;
    }
    ArLog::log(ArLog::Normal,"Robot connected");


    laserConnector->setupLaser(&sick);

    if(!laserConnector->connectLaser(&sick))
    {
        ArLog::log(ArLog::Terse,"Ops... falha ao conectar os lasers do robo.");
        return false;
    }

    ArLog::log(ArLog::Normal,"Laser connected");

    sick.runAsync();
    this->runAsync(true);
    ArUtil::sleep(500);
    this->lock();
    this->enableMotors();
    this->unlock();
    ArUtil::sleep(500);
    ArLog::log(ArLog::Normal,"Robot Ok.");

    return true;
}

bool Robot::stop()
{
    ArRobot::stop();
   // this->disableMotors();
}

bool Robot::shutdown()
{
    this->stop();
    this->stopRunning();
    this->waitForRunExit();
    Aria::shutdown();

    return true;
}

Robot::~Robot()
{
    this->shutdown();
}

void Robot::readingSensors()
{
    if(this->isConnected() && sick.isConnected())
    {
        sick.lockDevice();
        lasers = sick.getRawReadingsAsVector();
        sick.unlockDevice();
    }
}

int Robot::getLaserRange(int angle)
{
    if( !this->lasers || (this->lasers->size() <= angle))
        return 0;

    return this->lasers->at(angle).getRange();
}
vector<ArSensorReading>* Robot::getLaserRanges()
{
    return this->lasers;
}

int Robot::getSonarRange(int id_sonar)
{
    if(id_sonar > 8)
        return 0;
    return ArRobot::getSonarRange(id_sonar);
}

double Robot::getNorth()
{
    return ArRobot::getTh();
}

void Robot::move(int distanceMM)
{
    ArLog::log(ArLog::Normal,"Movendo: %d",distanceMM);
    this->lock();
    ArRobot::move(distanceMM);
    this->unlock();
}

void Robot::rotate(int degrees)
{
    ArLog::log(ArLog::Normal,"Rotacionando: %d",degrees);
    this->lock();
    ArRobot::setDeltaHeading(degrees);
    this->unlock();
}

