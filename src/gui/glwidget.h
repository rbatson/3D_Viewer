#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GL_SILENCE_DEPRECATION

#ifdef __cplusplus
extern "C" {
#endif
#include "../main/s21_parser.h"
#include "../main/s21_afin_transf.h"
#ifdef __cplusplus
}
#endif

#include <QOpenGLWidget>
#include <QtOpenGL>
#include <QTimer>
#include <QWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QFileDialog>
#include <QMessageBox>
#if defined(linux) || defined(_WIN32)
#include <GL/glut.h>
#else
#include <GLUT/GLUT.h>
#endif


class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();
public slots:
    void readObjFile();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
//    void wheelEvent(QWheelEvent *event) override;

private:

    data_t *objDraw = nullptr;
    unsigned int* polygonIndices = nullptr;
    unsigned int totalIndices;

    float xRot, yRot, mLastRotY, mLastRotX;
    QPointF mPos;

    void drawObj();
    void reshapePolygonIndices();
    void drawCube(float a);

};

#endif // GLWIDGET_H
