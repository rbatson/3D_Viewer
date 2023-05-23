#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

GLWidget::~GLWidget()
{

}


void GLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
}

void GLWidget::resizeGL(int w, int h)
{

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1, 3);

}

void GLWidget::paintGL()
{
    glClearColor(0, 1, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0, 0, -2);

    glRotatef(yRot, 0, 1, 0);
    glRotatef(xRot, 1, 0, 0);

    drawCube(0.5);

}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    mPos = event->position();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    yRot = mLastRotY + event->position().x() - mPos.x();
    xRot = mLastRotX + event->position().y() - mPos.y();

    update();

}
void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mLastRotX = xRot;
    mLastRotY = yRot;
    mPos = event->position();
}


void GLWidget::drawCube(float a){
    float ver_cub[] = {
        -a,-a, a,        a,-a, a,        a, a, a,       -a, a, a,
         a,-a,-a,       -a,-a,-a,       -a, a,-a,        a, a,-a,
        -a,-a,-a,       -a,-a, a,       -a, a, a,       -a, a,-a,
         a,-a, a,        a,-a,-a,        a, a,-a,        a, a, a,
        -a,-a, a,       -a,-a,-a,        a,-a,-a,        a,-a, a,
        -a, a, a,        a, a, a,        a, a,-a,       -a, a,-a
    };

    float color_arr[] = {
        1, 0, 0,         1, 0, 0,         1, 0, 0,         1, 0, 0,
        0, 0, 1,         0, 0, 1,         0, 0, 1,         0, 0, 1,
        1, 1, 0,         1, 1, 0,         1, 1, 0,         1, 1, 0,
        0, 1, 1,         0, 1, 1,         0, 1, 1,         0, 1, 1,
        1, 0, 1,         1, 0, 1,         1, 0, 1,         1, 0, 1,
        1, .5,.5,        1, .5,.5,        1, .5,.5,        1, .5,.5
    };
    glVertexPointer(3, GL_FLOAT, 0, &ver_cub);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColorPointer(3, GL_FLOAT, 0, &color_arr);
    glEnableClientState(GL_COLOR_ARRAY);

        glDrawArrays(GL_QUADS, 0, 24);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}
