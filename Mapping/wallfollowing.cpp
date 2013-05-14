#include "wallfollowing.h"

WallFollowing::WallFollowing(Robot *robot) :
    QObject()
{
    mRobot = robot;

    thread = new QThread();
    this->moveToThread(thread);

    connect(thread,SIGNAL(started()),this,SLOT(doWallFollowing()));
    connect(thread,SIGNAL(finished()),this,SLOT(finishWallFollowing()));

}

WallFollowing::~WallFollowing()
{
    this->stop();
    thread->exit();
}

void WallFollowing::start()
{
    ArLog::log(ArLog::Normal,"Start Wall Following");
    run = true;
    currentBehavior = RobotBehavior::WANDER;
    thread->start();
}

bool WallFollowing::isWallFollowingRunnig()
{
    return thread->isRunning();
}

void WallFollowing::stop()
{
    ArLog::log(ArLog::Normal,"Stopping Wall Following");
    run = false;
}

void WallFollowing::finishWallFollowing()
{
    ArLog::log(ArLog::Normal,"Stopped Wall Following");
}

void WallFollowing::doWallFollowing()
{

    if((mRobot == NULL) || (!mRobot->isConnected()) || (!mRobot->sick.isConnected()))
    {
        return;
    }


    double waitTime = 0.5, v = 300;
    int iteration = 0;

    PIDCorrection *left = new PIDCorrection(&mRobot->sick,30,60,1000);
    PIDCorrection *right = new PIDCorrection(&mRobot->sick,-30,-60,1000);
    PIDCorrection *front = new PIDCorrection(&mRobot->sick,-30,30,1000);

    srand(time(NULL));

    while(run)
    {

        mRobot->readingSensors();

        if(currentBehavior == RobotBehavior::WANDER)
        {
            if(left->getDistance() < left->desiredValue*1.5)
            {
                currentBehavior = RobotBehavior::WALL_LEFT;
            }
            else if(right->getDistance() < right->desiredValue*1.5)
            {
                if(currentBehavior == RobotBehavior::WANDER || (right->getDistance() < left->getDistance()))
                {
                    currentBehavior = RobotBehavior::WALL_RIGHT;
                }
            }

            //Se ainda continuar vagando... vague!
            if(currentBehavior == RobotBehavior::WANDER)
            {
                if(iteration % 20 == 0)
                {
                    mRobot->rotate((rand()%90) - 45);
                }
                mRobot->move(250);
            }
        }

        if(currentBehavior == RobotBehavior::WALL_LEFT)
        {
            int cf = front->getCorrection(waitTime),
                cl = left->getCorrection(waitTime);
            /*if(fabs(cf) < fabs(cl))
                this->walkWithPIDCorrection(v,cf);
            else
            */
                this->walkWithPIDCorrection(v,cl);
        }
        else if(currentBehavior == RobotBehavior::WALL_RIGHT)
        {
            int cf = front->getCorrection(waitTime),
                cr = right->getCorrection(waitTime);
            /*if(fabs(cf) < fabs(cr))
                this->walkWithPIDCorrection(v,cf);
            else*/
                this->walkWithPIDCorrection(v,cr);
        }
        iteration++;
        iteration %= (int)(60/waitTime);
        ArLog::log(ArLog::Normal,"Rodando");
        ArUtil::sleep(1000*waitTime);

    }
    mRobot->stop();
    ArLog::log(ArLog::Normal,"Finish");
    thread->exit();
}

void WallFollowing::walkWithPIDCorrection(double velocity, double correction)
{
    double e1 = 0, e2 = 0;
    double v1 = velocity, v2 = velocity;

    if(correction > 0)
    {
        e1 = correction;
    }
    else
    {
        e2 = -correction;
    }

    if(correction > v1)
    {
        e1 = v1;
    }
    else if(correction < -v1)
    {
        e2 = v2;
    }
    mRobot->setVel2(v1-e1,v2-e2);

    ArLog::log(ArLog::Terse,"Menor distancia: %f",mRobot->sick.currentReadingPolar(30,60));
    ArLog::log(ArLog::Terse,"Coeficiente: %f",correction);
}
