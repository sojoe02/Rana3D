#include "glwidget.h"
#include "utility.h"
#include <GL/glut.h>
#include <QMouseEvent>
#include <iostream>

void myBalls(GLdouble size, GLfloat dispX, GLfloat dispY,GLfloat dispZ,float R,float G,float B){
    float currentColor[3];
    glGetFloatv(GL_CURRENT_COLOR,currentColor);
    glColor3f(R/255, G/255, B/255);
    glTranslatef(dispX,dispY, dispZ); //orig xzy
    glutWireSphere(size,15,15);
    glColor3f(currentColor[0],currentColor[1],currentColor[2]);
    glTranslatef(-dispX,-dispY,-dispZ);
}

void GLWidget::drawAgents(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    for(const auto &current : spheres)
    {
        myBalls(0,0,0,0,0,0,0);
        float red,green,blue;
        //if (current.second.color.red>255 || (current.second.color.red<1&&current.second.color.red!=0)){
        //    red = 255;
        //}else{
            red=current.second.color.red;
        //}
        //if (current.second.color.green>255 || (current.second.color.green<1&&current.second.color.green!=0)){
        //    green = 255;
        //}else{
            green=current.second.color.green;
        //}
        //if (current.second.color.blue>255 || (current.second.color.blue<1&&current.second.color.blue!=0)){
        //    blue = 255;
        //}else{
            blue=current.second.color.blue;
        //}
        myBalls(current.second.radius,current.second.x,current.second.y,current.second.z,red,green,blue);
        //std::cout<<"Colour is "<<current.second.color.red<<", "<<current.second.color.green<<", "<<current.second.color.blue<<"   with radius "<<current.second.radius<<std::endl;
    }

    glPopMatrix();
    glFlush();
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        update();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        update();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        update();
    }
}

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_xRot(0),
      m_yRot(0),
      m_zRot(0)
{
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    place=2;
    up = true;
    zoom = 1;
    mover=0;

}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); // reset transformation state
    glTranslatef(0.0, 0.0, -10.0); // apply translation to transformation
    glRotatef(m_xRot / 16.0, 1.0, 0.0, 0.0); // apply rotation on x
    glRotatef(m_yRot / 16.0, 0.0, 1.0, 0.0); // apply rotation on x
    glRotatef(m_zRot / 16.0, 0.0, 0.0, 1.0); // apply rotation on x
    glScalef((GLfloat)zoom,(GLfloat)zoom,(GLfloat)zoom);

    if(mover>2&&up==true){
        up=false;
    }else if(mover<-2&&up==false){
        up=true;
    }else if(up==true){
        mover += 0.1;
    }else{
        mover -= 0.1;
    }

    drawAgents();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,0,0,0,0,1,0);

}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();

}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
    m_lastPos = event->pos();
}
