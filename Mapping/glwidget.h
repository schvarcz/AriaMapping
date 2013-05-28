#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QObject>
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFormat>
#include <QResizeEvent>
#include <iostream>
#include <mapping.h>
#include <glthread.h>

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
    void setMapping(Mapping *mapping);
    Mapping* mapping();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    
private:
    int width, height;
    Mapping *mMapping = NULL;
    GLThread mGLThread;

public slots:
    void updateScreen();

signals:
    
};

#endif // GLWIDGET_H
