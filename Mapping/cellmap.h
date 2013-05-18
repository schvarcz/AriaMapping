#ifndef CELLMAP_H
#define CELLMAP_H

#include <QObject>
#include <QtCore>
#include <QtGui>

class CellMap : public QObject
{
    Q_OBJECT
public:
    explicit CellMap();
    QGraphicsPolygonItem *polygonCell();
    void setPolygonCell(QGraphicsPolygonItem *polygon);

    float cellValue();
    void setCellValue(float value);

private:
    QGraphicsPolygonItem *poly = NULL;
    float value = 0.0;
    
public slots:

signals:

};

#endif // CELLMAP_H
