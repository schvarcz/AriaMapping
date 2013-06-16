#include "cellmap.h"

CellMap::CellMap() :
    himmCValue(-1),
    pVazia(0.5),
    pOcupada(0.5),
    QObject()
{
}

void CellMap::setCellValue(float value)
{
    if(poly && (this->value != value))
    {
        poly->setBrush(QBrush(QColor(255*value,255*value,255*value)));
        //poly->update(poly->boundingRect());
    }
    this->value = value;
}

float CellMap::cellValue()
{
    return value;
}

void CellMap::addHimmProbability()
{
    himmCValue += 3;
    if(himmCValue > HIMM_MAX)
        himmCValue = HIMM_MAX;
}

void CellMap::subHimmProbability()
{
    himmCValue -= 1;
    if(himmCValue < HIMM_MIN)
        himmCValue = HIMM_MIN;
}

int CellMap::himmValue()
{
    return himmCValue;
}

float CellMap::himmProbability()
{
    return himmCValue < 0 ? himmCValue :1.0 - ((float)himmCValue)/((float)(HIMM_MAX-HIMM_MIN));
}

double CellMap::probabilidadeOcupada()
{
    return pOcupada;
}

double CellMap::probabilidadeVazia()
{
    return pVazia;
}

void CellMap::setProbabilidadeOcupada(double value)
{
    if(value >0 && value <1.0)
    {
        pOcupada = value;
        pVazia = 1.0-pOcupada;
    }
}

void CellMap::setProbabilidadeVazia(double value)
{
    if(value >0 && value <1.0)
    {
        pVazia = value;
        pOcupada = 1.0-pVazia;
    }
}

double CellMap::isBayesProbability()
{
    return pOcupada != 0.5?pOcupada>0.1?1.0:0.0:0.5;
}

void CellMap::setPolygonCell(QGraphicsPolygonItem *polygon)
{
    if(poly)
    {
        delete poly;
    }
    poly = polygon;
    poly->setBrush(QBrush(QColor(255*value,255*value,255*value)));
}

void CellMap::operator =(float value)
{
    this->setCellValue(value);
}

float CellMap::operator *(float value)
{
    return this->value * value;
}

float CellMap::operator *(int value)
{
    return this->value * value;
}

bool CellMap::operator <(float value)
{
    return this->value < value;
}

bool CellMap::operator >(float value)
{
    return this->value > value;
}

bool CellMap::operator <=(float value)
{
    return this->value <= value;
}

bool CellMap::operator >=(float value)
{
    return this->value >= value;
}

bool CellMap::operator ==(float value)
{
    return this->value == value;
}

bool CellMap::operator !=(float value)
{
    return this->value != value;
}
