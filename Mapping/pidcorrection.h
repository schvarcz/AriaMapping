#ifndef PIDCORRECTION_H
#define PIDCORRECTION_H

#include <ArRangeDevice.h>

class PIDCorrection
{
public:
    PIDCorrection(ArRangeDevice *rangeDevice, double startAngle, double finishAngle, double desiredValue);
    double getCorrection(double deltaTime);
    double getDistance();
    ArRangeDevice *rangeDevice;
    double integral, erro_anterior,desiredValue ,
        kp, ki, kd, startAngle, finishAngle;
private:

};

#endif // PIDCORRECTION_H
