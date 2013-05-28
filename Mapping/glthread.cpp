#include "glthread.h"

GLThread::GLThread(GLWidget *glWidget) :
    QThread(),
    mGLWidget(glWidget)
{
    doRendering = true;
    doResize = false;
}

void GLThread::resizeViewport(QSize size)
{
    qDebug() << "Resizing Viewport";
    mSize = size;
    doResize = true;
}

void GLThread::stop()
{
    doRendering = false;
}

void GLThread::run()
{

    while(doRendering)
    {
        mGLWidget->makeCurrent();
        mGLWidget->swapBuffers();
        mGLWidget->doneCurrent();
        msleep(66);
    }
}
