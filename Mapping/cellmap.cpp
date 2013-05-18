#include "cellmap.h"

CellMap::CellMap() :
    QObject()
{
}

void CellMap::setCellValue(float value)
{
    this->value = value;
    if(poly)
    {
        poly->setBrush(QBrush(QColor(255*value,255*value,255*value)));
    }
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
