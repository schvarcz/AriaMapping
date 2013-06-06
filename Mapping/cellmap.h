#ifndef CELLMAP_H
#define CELLMAP_H

#include <QObject>
#include <QtCore>
#include <QtGui>

#define HIMM_MAX 15
#define HIMM_MIN 0

class CellMap : public QObject
{
    Q_OBJECT
public:
    explicit CellMap();
    QGraphicsPolygonItem *polygonCell();
    void setPolygonCell(QGraphicsPolygonItem *polygon);

    float cellValue();
    void setCellValue(float value);

    void addHimmProbability();
    void subHimmProbability();
    int himmValue();
    float himmProbability();

    /* Apenas para facilitar uma transição entre um grid de floats e um grid de CellMap */
    void operator =(float value);
    float operator *(float value);
    float operator *(int value);
    bool operator <(float value);
    bool operator >(float value);
    bool operator !=(float value);
    bool operator ==(float value);
    bool operator <=(float value);
    bool operator >=(float value);

private:
    QGraphicsPolygonItem *poly = NULL;
    float value = 0.0;
    int himmCValue;
    
public slots:

signals:

};

#endif // CELLMAP_H
