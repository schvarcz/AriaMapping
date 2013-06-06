#include "robot.h"

Robot::Robot(const char * name, bool ignored,
             bool doSigHandle,
             bool normalInit, bool addAriaExitCallback):
    ArRobot(name, ignored, doSigHandle, normalInit, addAriaExitCallback),
    QObject()
{
    int argc = 0;
    char *argv = NULL;
    parser = new ArArgumentParser(&argc,&argv);
    robotConnector = new ArRobotConnector(parser,this);
    laserConnector = new ArLaserConnector(parser,this,robotConnector);

    sonares = new vector<ArSensorReading>();

    /*thread = new QThread();
    connect(thread,SIGNAL(started()),this,SLOT(keepReading()));
    connect(thread,SIGNAL(finished()),this,SLOT(stopReading()));
    this->moveToThread(thread);
    run = true;
    thread->start();*/
}

bool Robot::start()
{
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
    //run = true;
    //thread->start();
    ArLog::log(ArLog::Normal,"Robot Ok.");

    return true;
}

bool Robot::stop()
{
    run = false;
    ArRobot::stop();
   // this->disableMotors();
}

bool Robot::shutdown()
{
    this->stop();
    this->stopRunning();
    this->waitForRunExit();

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
        /*while((!ArRobot::isMoveDone()) || (!ArRobot::isHeadingDone()))
            ArUtil::sleep(33);*/
        this->lock();
        sick.lockDevice();
        if(lasers)
        {
            lasers->clear();
        }
        lasers = sick.getRawReadingsAsVector();
        sick.unlockDevice();

        this->lock();
        if(sonares)
        {
            sonares->clear();
        }
        for(int i=0;i<=7;i++)
        {
            sonares->push_back(*this->getSonarReading(i));
        }
        this->unlock();
        this->unlock();
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
    vector<ArSensorReading> *ret = new vector<ArSensorReading>();
    sick.lockDevice();
    for (int i=0;i<=180;i++)
    {
        ret->push_back(ArSensorReading(this->lasers->at(i)));
    }
    sick.unlockDevice();
    return ret;
}

int Robot::getSonarRange(int id_sonar)
{
    if(id_sonar > 8)
        return 0;
   /* while((!ArRobot::isMoveDone()) || (!ArRobot::isHeadingDone()))
        ArUtil::sleep(33);*/
    this->lock();
    int ret = ArRobot::getSonarRange(id_sonar);
    this->unlock();
    return ret;
}

vector<ArSensorReading>* Robot::getSonarRanges()
{
    vector<ArSensorReading> *ret = new vector<ArSensorReading>();
    this->lock();
    for(int i =0;i<=7;i++)
    {
        ret->push_back(ArSensorReading(sonares->at(i)));
    }
    this->unlock();
    return ret;
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


void Robot::keepReading()
{
    ArLog::log(ArLog::Normal,"Reading started");
    while(run)
    {
        this->readingSensors();
        ArUtil::sleep(33);
    }
    thread->exit();
}

void Robot::stopReading()
{
    ArLog::log(ArLog::Normal,"Reading stopped");
}
