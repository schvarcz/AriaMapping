#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers),parent),
    mGLThread(this)
{
    cout << parent->height() << endl;
    cout << parent->width() << endl;
    //setAutoBufferSwap(false);
}

GLWidget::~GLWidget()
{
}

void GLWidget::startRendering()
{
    mGLThread.start();
}

void GLWidget::stopRendering()
{
    mGLThread.stop();
    mGLThread.wait();
}

QSize GLWidget::minimumSize() const
{
    return QSize(400,400);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400,400);
}

QSize GLWidget::maximumSize() const
{
    return QSize(400,400);
}

void GLWidget::initializeGL()
{
    cout << "initialize GL" << endl;

    qglClearColor(QColor(50,50,50));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWidget::paintGL()
{
    cout << "Paint GL" << endl;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //qglColor(QColor(200,200,200));
    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    int size = 450;
    glVertex2f(-size,-size);
    glVertex2f(-size,size);
    glVertex2f(size,size);
    glVertex2f(size,-size);

    glEnd();

    if(mMapping)
    {
        mMapping->render();
    }

    //startRendering();
}

void GLWidget::resizeGL(int w, int h)
{
    cout << "Resize GL" << endl;
    width = w;
    height = h;

    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int size = 500;
    glOrtho(-size, size, -size, size,-1,1);
    if(!mGLThread.isRunning())
        updateGL();
}

void GLWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "Resizing Event";
    QGLWidget::resizeEvent(event);
    //mGLThread.resizeViewport(event->size());
    this->resizeGL(event->size().width(),event->size().height());
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    //Done by GLThread
}

void GLWidget::closeEvent(QCloseEvent *event)
{
    stopRendering();
    QGLWidget::closeEvent(event);
}

void GLWidget::resize(int w, int h)
{
    qDebug() << "Resizing";
    mGLThread.resizeViewport(QSize(w,h));
    QGLWidget::resize(w,h);
    this->resizeGL(w,h);
}

void GLWidget::setMapping(Mapping *mapping)
{
    mMapping = mapping;
    connect(mMapping,SIGNAL(updateScreen()),this,SLOT(updateScreen()));
}

Mapping* GLWidget::mapping()
{
    return mMapping;
}

void GLWidget::updateScreen()
{
    //startRendering();

    if(!mGLThread.isRunning())
        this->updateGL();

}
