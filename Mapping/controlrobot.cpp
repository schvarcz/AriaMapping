#include "controlrobot.h"

ControlRobot::ControlRobot(CampoPotencial *campo, Robot *robot) :
    QThread()
{
    mCampoPotencial = campo;
    mRobot = robot;
}

void ControlRobot::MoveRobot()
{

}

void ControlRobot::run()
{
    double celRange = 2*30000/MAP_LENGTH_WORLD;

    while(true)
    {
        int x = mRobot->getX()/celRange + MAP_LENGTH_WORLD/2, y = MAP_LENGTH_WORLD/2 - mRobot->getY()/celRange;
        double dX=0.0, dY=0.0;
        if((x>0) && (x<MAP_LENGTH_WORLD))
            dX = (mCampoPotencial->mPotencial[x+1][y] - mCampoPotencial->mPotencial[x-1][y])/2;
        if((y>0) && (y<MAP_LENGTH_WORLD))
            dY = (mCampoPotencial->mPotencial[x][y-1] - mCampoPotencial->mPotencial[x][y+1])/2;

        //if((dY != 0.0) || (dX!=0.0))
        {
            if(fabs(dY)<fabs(dX))
            {
                dX = 0;
            }
            else
            {
                dY = 0;
            }

            double th = atan2(dY,dX)*180/M_PI;
            double rTh = mRobot->getTh();
            //cout << "Minha rotacao:  " << th << "  dx: " << dX << "  dy: " << dY << "  x: " << x << "  y: " << y << "  potencial: " << mCampoPotencial->mPotencial[x][y] << endl;
            //mRobot->rotate(rTh-th);
            double rad = (th-rTh)*M_PI/180.0;
            if(fabs(rad) > M_PI/6)
            {
                rad = M_PI/2*rad/rad;
            }
            mCampoPotencial->angleAndar=(th-rTh);
            cout << "rad: " << rad << "   left: " << 50-50*sin(rad) << "  right: "<< 50+50*sin(rad) << endl;
            mRobot->setVel2(50+50*sin(rad),50-50*sin(rad));
            /*if(rTh-th<10)
            {
                mRobot->move(100);
            }
            else
            {
                //mRobot->setVel2(0,0);
                double rot = rTh-th;
                rot = min(10.0,rot);
                rot = max(-10.0,rot);
                mRobot->rotate(rot);
            }

            /*if(rTh-th < 45)
                mRobot->move(100);*/
        }
        msleep(500);
    }
}
