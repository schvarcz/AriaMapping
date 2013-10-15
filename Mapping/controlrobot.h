#ifndef CONTROLROBOT_H
#define CONTROLROBOT_H

#include <QThread>
#include <campopotencial.h>
#include <robot.h>
#include <iostream>

using namespace std;

class ControlRobot : public QThread
{
    Q_OBJECT
public:
    explicit ControlRobot(CampoPotencial *campo, Robot *robot);
    void MoveRobot();
    void run();

    CampoPotencial *mCampoPotencial;
    Robot *mRobot;
    
signals:
    
public slots:
    
};

#endif // CONTROLROBOT_H
