#include "cellmap.h"

CellMap::CellMap() :
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
