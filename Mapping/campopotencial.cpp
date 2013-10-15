#include "campopotencial.h"

CampoPotencial::CampoPotencial(Mapping *map, int mapSize): QThread(), IRenderMap()
{
    angleAndar = 0.0;
    mMapping = map;
    mMapSize = mapSize;
    celRange = 100;
    Dmin = celRange;
    Dmax = celRange/cos(M_PI*45.0/180.0);
    Dobstacle = mMapSize*mMapSize*Dmax*2;
    mPotencial = new double*[mMapSize];
    for(int x=0;x<mMapSize;x++)
    {
        mPotencial[x] = new double[mMapSize];
        for(int y=0;y<mMapSize;y++)
        {
            mPotencial[x][y] = -Dobstacle;
        }
    }
}

void CampoPotencial::run()
{
    gettimeofday(&begin,NULL);
    time = true;
    bool relaxa = true;
    float m = 0.0;
    while(true)
    {

        //qDebug() << "vamos computar!";
        if(relaxa)
        {
            m = ComputaDistancia();
            //cout << m << endl;
            //if(m<40000000)
             //   relaxa = false;
        }


        if((time) && (!relaxa))
        {
            gettimeofday(&end,NULL);

            qDebug() << "Grafo criado!";
            qDebug() << "Tempo criacao: " << ((double)(end.tv_usec-begin.tv_usec))/1000000 + (double)(end.tv_sec-begin.tv_sec);
            time = false;
        }
    }
    qDebug() << "saindo!";
}

float CampoPotencial::ComputaDistancia()
{

    float ret =0;
    //cout << "computando... " << endl;
    for(int x=1;x<mMapSize-1;x++)
    {
        for(int y=0;y<mMapSize;y++)
        {
            //qDebug() << x << y << mMapSize << mMap[x][y] ;
            float value;
            //value = mMapping->mapCell[x][y].isHimmProbability();
            //value = mMapping->mapCell[x][y].isBayesProbability();
            value = mMapping->mapCell[x][y].cellValue();
            if(value == 0.0)
            {
                mPotencial[x][y] = Dobstacle;
            }
            /*
            else if(value == 0.7)
            {
                //mPotencial[x][y] = -Dobstacle;
                double n = (mPotencial[x-1][y]+mPotencial[x+1][y]+mPotencial[x][y-1]+mPotencial[x][y+1])/4.0;
                ret += pow(n-mPotencial[x][y],2);
                mPotencial[x][y] = n;
            }*/
            else// if(value == 1.0)
            {
                double n = (mPotencial[x-1][y]+mPotencial[x+1][y]+mPotencial[x][y-1]+mPotencial[x][y+1])/4.0;
                ret += pow(n-mPotencial[x][y],2);
                mPotencial[x][y] = n;
            }
        }
    }
    return ret;
}

void CampoPotencial::MoveRobot()
{

}

double CampoPotencial::distance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}

void CampoPotencial::render()
{
    //Desenhando o mapa em si
    //qDebug() << "rendering" << endl;
    glBegin(GL_QUADS);
    for(int x=0;x<mMapSize-1;x++)
    {
        for(int y=0;y<mMapSize-1;y++)
        {

            float xP = (x-1-mMapSize/2)*celRange, yP = (mMapSize/2 - y+1)*celRange;
            //Cor de X ,Y
            paleta.updatePorcentagem(mPotencial[x][y]/Dobstacle);
            glColor3f(paleta.cor->redF(),paleta.cor->greenF(),paleta.cor->blueF());
            glVertex3f(xP,yP+celRange,15000.0f*mPotencial[x][y]/Dobstacle);
            //Cor de X ,Y
            paleta.updatePorcentagem(mPotencial[x+1][y]/Dobstacle);
            glColor3f(paleta.cor->redF(),paleta.cor->greenF(),paleta.cor->blueF());
            glVertex3f(xP+celRange,yP+celRange,15000.0f*mPotencial[x+1][y]/Dobstacle);
            //Cor de X ,Y
            paleta.updatePorcentagem(mPotencial[x+1][y+1]/Dobstacle);
            glColor3f(paleta.cor->redF(),paleta.cor->greenF(),paleta.cor->blueF());
            glVertex3f(xP+celRange, yP,15000.0f*mPotencial[x+1][y+1]/Dobstacle);
            //Cor de X ,Y
            paleta.updatePorcentagem(mPotencial[x][y+1]/Dobstacle);
            glColor3f(paleta.cor->redF(),paleta.cor->greenF(),paleta.cor->blueF());
            glVertex3f(xP,yP,15000.0f*mPotencial[x][y+1]/Dobstacle);
        }
    }

    glEnd();

    mMapping->drawRobot();


    glTranslated(mMapping->xRobo,mMapping->yRobo,0.0);
    glRotated(mMapping->thRobo,0.0,0.0,1.0);

    glRotated(angleAndar,0.0,0.0,1.0);

    glBegin(GL_QUADS);
    glColor3f(0.0f,1.0f,1.0f);
    glVertex3i(0.0f,0.0f,15000.0f+3.0);
    glVertex3i(40*20,0.0f,15000.0f+3.0);
    glVertex3i(40*20,3*20,15000.0f+3.0);
    glVertex3i(0*20,3*20,15000.0f+3.0);
    glEnd();

    glRotated(-angleAndar,0.0,0.0,1.0);

    glRotated(-mMapping->thRobo,0.0,0.0,1.0);
    glTranslated(-mMapping->xRobo,-mMapping->yRobo,0.0);

}


void CampoPotencial::drawRobot()
{
    /*
    glTranslated(xRobo,yRobo,0.0);
    glRotated(thRobo-90,0.0,0.0,1.0);

    glColor3f(1.0f,0.0f,0.0f);


    int fator = 20;

    //Draw red part (body)
    glBegin(GL_QUADS);

    glVertex3i(-10*fator,5*fator,1.0);
    glVertex3i(10*fator,5*fator,1.0);
    glVertex3i(10*fator,-5*fator,1.0);
    glVertex3i(-10*fator,-5*fator,1.0);

    glVertex3i(-5*fator,12*fator,1.0);
    glVertex3i(5*fator,12*fator,1.0);
    glVertex3i(5*fator,-12*fator,1.0);
    glVertex3i(-5*fator,-12*fator,1.0);

    glEnd();

    glBegin(GL_TRIANGLES);

    glVertex3i(-5*fator,-12*fator,1.0);
    glVertex3i(-5*fator,-5*fator,1.0);
    glVertex3i(-10*fator,-5*fator,1.0);

    glVertex3i(5*fator,12*fator,1.0);
    glVertex3i(5*fator,5*fator,1.0);
    glVertex3i(10*fator,5*fator,1.0);

    glVertex3i(-5*fator,12*fator,1.0);
    glVertex3i(-5*fator,5*fator,1.0);
    glVertex3i(-10*fator,5*fator,1.0);


    glVertex3i(5*fator,-12*fator,1.0);
    glVertex3i(5*fator,-5*fator,1.0);
    glVertex3i(10*fator,-5*fator,1.0);

    glEnd();

    //Draw black box (deck)
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);

    glVertex3i(-5*fator,-12*fator,2.0);
    glVertex3i(5*fator,-12*fator,2.0);
    glVertex3i(5*fator,0*fator,2.0);
    glVertex3i(-5*fator,0*fator,2.0);

    //Draw blue box (laser)
    glColor3f(0.0f,0.0f,1.0f);
    glVertex3i(-5*fator,10*fator,2.0);
    glVertex3i(5*fator,10*fator,2.0);
    glVertex3i(5*fator,5*fator,2.0);
    glVertex3i(-5*fator,5*fator,2.0);

    glEnd();

    glRotated(90-thRobo,0.0,0.0,1.0);
    glTranslated(-xRobo,-yRobo,0.0);
    */
}
