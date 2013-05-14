#ifndef WALLFOLLOWING_H
#define WALLFOLLOWING_H

#include <QObject>
#include <QThread>
#include <robot.h>
#include <pidcorrection.h>
#include <vector>

namespace RobotBehavior
{
    enum Behavior
    {
        WANDER,
        WALL_LEFT,
        WALL_RIGHT
    };
}

using namespace RobotBehavior;

class WallFollowing : public QObject
{
    Q_OBJECT
public:
    explicit WallFollowing(Robot *robot=NULL);
    void start();
    void stop();
    bool isWallFollowingRunnig();
    ~WallFollowing();

private:
    Robot *mRobot;
    QThread *thread;
    bool run = true;
    Behavior currentBehavior;

    void walkWithPIDCorrection(double velocity,double correction);

public slots:
    void doWallFollowing();
    void finishWallFollowing();

signals:

};

#endif // WALLFOLLOWING_H
