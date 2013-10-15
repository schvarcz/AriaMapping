#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers),parent),
    mGLThread(this),
    mRendingMap(NULL)
{
    cout << parent->height() << endl;
    cout << parent->width() << endl;
    //setAutoBufferSwap(false);

    tx = ty = tz = 0.0f;
    rx = ry = rz = 0.0f;
    oldMousePosition = NULL;
    startRendering();
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

bool GLWidget::isRendering()
{
    return mGLThread.isRunning();
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

    qglClearColor(QColor(200,200,200));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/*
    GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};

    GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};	   // "cor"
    GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
    GLfloat posicaoLuz[4]={0.0, 0.0, 15000.0, 1.0};

    // Capacidade de brilho do material
    GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
    GLint especMaterial = 60;

    // Habilita o modelo de colorização de Gouraud
    glShadeModel(GL_SMOOTH);

    // Define a refletância do material
    //glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    // Define a concentração do brilho
    //glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);


    // Define os parâmetros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    //glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
    // Habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    //Habilita o uso de iluminação
    glEnable(GL_LIGHTING);
    // Habilita a luz de número 0
    glEnable(GL_LIGHT0);
    // Habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);*/
}

void GLWidget::paintGL()
{
    cout << "Paint GL" << endl;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glTranslated(tx,ty,tz);
    //qDebug()
    glRotatef(rx,1.0f,0.0f,0.0f);
    glRotatef(ry,0.0f,1.0f,0.0f);
    glRotatef(rz,0.0f,0.0f,1.0f);
    int size = 15000;
    float aspect = (float)width/(float)height;
    glOrtho(-size*aspect, size*aspect, -size, size,-3*size,3*size);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(mRendingMap)
    {
        mRendingMap->render();
    }

}

void GLWidget::resizeGL(int w, int h)
{
    cout << "Resize GL" << endl;
    width = w;
    height = h;

    int side = qMin(w, h);
    //glViewport((w - side) / 2, (h - side) / 2, side, side);
    glViewport(0, 0, w, h);

    float aspect = w/h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glTranslated(tx,ty,tz);
    glRotatef(rx,1.0f,0.0f,0.0f);
    glRotatef(ry,0.0f,1.0f,0.0f);
    glRotatef(rz,0.0f,0.0f,1.0f);
    int size = 15000;

    glOrtho(-size*aspect, size*aspect, -size, size,-3*size,3*size);
    if(!mGLThread.isRunning())
        updateGL();
}

void GLWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "Resizing Event";
    QGLWidget::resizeEvent(event);
    //mGLThread.resizeViewport(event->size());
    //this->resizeGL(event->size().width(),event->size().height());
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    //Done by GLThread
    if(!this->isRendering())
    {
        startRendering();
    }
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
    //this->resizeGL(w,h);
}

void GLWidget::setRendingMap(IRenderMap *rendingMap)
{
    mRendingMap = rendingMap;
}

IRenderMap* GLWidget::rendingMap()
{
    return mRendingMap;
}

void GLWidget::updateScreen()
{
    //startRendering();

    if(!mGLThread.isRunning())
        this->updateGL();

}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    delete oldMousePosition;
    oldMousePosition = NULL;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{

    double side = (double)min(width,height);

    double size = 1.0;
    double x = event->pos().x(), y =-event->pos().y();
    if(oldMousePosition)
    {
        x -= oldMousePosition->x();
        y += oldMousePosition->y();
        x *= (2*size)/side;
        y *= (2*size)/side;

        if(event->modifiers() == Qt::ControlModifier)
        {
            qDebug() << 180.0*x;
            if(fabs(x) < fabs(y))
            {
                rx += 180*y;
            }
            else
            {
                ry -= 180*x;
            }
        }
        else
        {
            tx += x;
            ty += y;
        }
    }
    oldMousePosition = new QPoint(event->pos().x(),event->pos().y());
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    double side = (double)min(width,height);

    double size = 15000.0;
    double x = event->x() - (double)width/2, y =(double)height/2 - event->y();
    x *= (2*size)/side;
    y *= (2*size)/side;
}

