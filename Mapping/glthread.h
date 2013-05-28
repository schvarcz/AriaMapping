#ifndef GLTHREAD_H
#define GLTHREAD_H

#include <QObject>
#include <QThread>
#include <QGLWidget>

class GLWidget;

class GLThread : public QThread
{
    Q_OBJECT
public:
    explicit GLThread(GLWidget *glWidget);
    void resizeViewport(QSize size);
    void run();
    void stop();
    
private:
    bool doRendering, doResize;
    GLWidget *mGLWidget;
    QSize mSize;

signals:
    
public slots:
    
};

#include <glwidget.h>

#endif // GLTHREAD_H
