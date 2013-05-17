#include "pidcorrection.h"

PIDCorrection::PIDCorrection( ArRangeDevice *rangeDevice, double startAngle, double finishAngle, double desiredValue)
{
    this->rangeDevice = rangeDevice;
    this->startAngle = startAngle;
    this->finishAngle = finishAngle;
    this->desiredValue = desiredValue;

    //Bom 0.8 0 0.5
    //bom 1 0 0.5
    //Bom 0.8 0 0.3
    //.6 .002 .003 MUITO suave, mas ocila muita na linha reta
    this->kp = 0.8;
    this->ki = 0;
    this->kd = 0.03;
}

double PIDCorrection::getCorrection(double deltaTime)
{
    double erro = desiredValue - this->getDistance();
    integral += erro*deltaTime;
    double derivada = (erro - erro_anterior)/deltaTime;
    erro_anterior = erro;
    return kp*erro + ki*integral + kd*derivada;
}

double PIDCorrection::getDistance()
{
    return rangeDevice->currentReadingPolar(startAngle,finishAngle)-1000; //o 1000 seria o tamanho do robo.
}
