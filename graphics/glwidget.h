#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <GL/glu.h>
#include <GL/gl.h>
#include <QMatrix4x4>
#include "sphereitem.h"

class GLWidget : public QOpenGLWidget
{
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
    void paintGL();
    std::map<int,sphereItem> spheres;
    void resizeGL(int w, int h);
    double zoom;

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void cleanup();

protected:
    void initializeGL();
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void drawAgents();
    int m_xRot;
    int m_yRot;
    int m_zRot;
    QPoint m_lastPos;
    QMatrix4x4 m_world;
    int place;
    bool up;
    double mover;
};

#endif // GLWIDGET_H
