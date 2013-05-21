#ifndef ROBOT_H
#define ROBOT_H

#include <Aria.h>
#include <vector>
#include <QObject>
#include <QThread>

using namespace std;


class Robot : public QObject, public ArRobot
{
    Q_OBJECT

public:
    explicit Robot(const char * name = NULL, bool ignored = true,
          bool doSigHandle=true,
          bool normalInit = true, bool addAriaExitCallback = true);
    ~Robot();
    void move(int distanceMM);
    void rotate(int degrees);
    int getLaserRange(int angle);
    vector<ArSensorReading>* getLaserRanges();
    int getSonarRange(int id_sonar);
    double getNorth();
    void readingSensors();
    bool shutdown();
    bool start();
    bool stop();

    ArSick sick;

private:

    ArRobotConnector *robotConnector;
    ArLaserConnector *laserConnector;
    ArArgumentParser *parser;
    vector<ArSensorReading> *lasers = NULL;
    QThread *thread;
    bool run;

public slots:
    void keepReading();
    void stopReading();

signals:

};

#endif // ROBOT_H
