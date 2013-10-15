#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include <QObject>
#include <wallfollowing.h>
#include <robot.h>
#include <minimapping.h>
#include <QtCore>
#include <QtGui>
#include <mapping.h>
#include <campopotencial.h>
#include <controlrobot.h>

class Architecture : public QObject
{
    Q_OBJECT

public:
    explicit Architecture();
    ~Architecture();
    void startWallFollowing();
    void stopWallFollowing();
    void forward(int distance);
    void backward(int distance);
    void rotate(float degrees);
    Mapping* worldMap(){ return this->mWorldMap; }
    CampoPotencial* campoPotencial(){ return this->mCampoPotencial; }

private:
    Robot *mRobot;
    Mapping *mWorldMap;
    WallFollowing *wallFollowing;
    CampoPotencial *mCampoPotencial;
    ControlRobot *mControlRobot;

public slots:

signals:

};

#endif // ARCHITECTURE_H
