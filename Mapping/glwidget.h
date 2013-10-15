#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QObject>
#include <QtCore>
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFormat>
#include <QResizeEvent>
#include <iostream>
#include <glthread.h>
#include <math.h>
#include <irendermap.h>
#include <mapping.h>

using namespace std;

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();
    QSize minimumSize() const;
    QSize sizeHint() const;
    QSize maximumSize() const;
    void resize(int w, int h);
    void startRendering();
    void stopRendering();
    bool isRendering();
    void setRendingMap(IRenderMap *rendingMap);
    IRenderMap* rendingMap();
    void resizeGL(int w, int h);

protected:
    void initializeGL();
    void paintGL();
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    
private:
    int width, height;
    IRenderMap *mRendingMap;
    GLThread mGLThread;
    double tx,ty,tz,rx,ry,rz;
    QPoint *oldMousePosition;

public slots:
    void updateScreen();

signals:
    
};

#endif // GLWIDGET_H
