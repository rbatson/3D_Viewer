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
    glClearColor(0, 0, 0, 0);
    if (objDraw != nullptr){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(0, 0, -2);

        glRotatef(xRot, 1, 0, 0);
        glRotatef(yRot, 0, 1, 0);

        drawObj();
        //    drawCube(0.5);
    }

}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    mPos = event->position();
    mLastRotX = xRot;
    mLastRotY = yRot;
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

void GLWidget::readObjFile()
{
    objDraw = (data_t *)calloc(1, sizeof(data_t));
    char path_to_file[] = "/Users/Blangish/Desktop/rbatson3D/src/test_obj/cube.obj";
    s21_parser(objDraw, path_to_file);
}

void GLWidget::drawObj(){
//    double a = 0.5;
//    double ver_cub[] = {
//         a,-a,-a,        a,-a, a,       -a,-a, a,       -a, -a, -a,
//         a, a,-a,        a, a, a,       -a, a, a,        -a, a,-a
//    };

    double *test = (double *)calloc(25, sizeof(double));
    for (int i = 0; i < 24; i++) {
        int k = (i + 3) / 3;
        int j = i % 3;
        test[i] = objDraw->matrix_3d.matrix[k][j];
    }


    reshapePolygonIndices();
    glVertexPointer(3, GL_DOUBLE, 0, test);//doesn't work, but if replace with ver_cub works fine

    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawElements(GL_LINES, totalIndices /*size of polygon_indices*/, GL_UNSIGNED_INT, polygonIndices);

    glDisableClientState(GL_VERTEX_ARRAY);
}

void GLWidget::reshapePolygonIndices(){
    if (polygonIndices != nullptr) {
        free(polygonIndices);
        polygonIndices = nullptr;
    }
    unsigned int polygonAmount = objDraw->count_of_facets;
    totalIndices = polygonAmount * 2  * 3 /*depends on the shape of a single polygon*/;

    polygonIndices = (unsigned int*)calloc(totalIndices, sizeof(unsigned int));

    for (size_t i = 1, k = 0; i <= polygonAmount; i++) {
      for (size_t j = 0, l = 0; j < objDraw->polygons[i].number_of_vertexes_in_facets; j++, l += 2) {
        polygonIndices[(k + l)] = objDraw->polygons[i].vertexes[j] - 1;
        if (j + 1 == objDraw->polygons[i].number_of_vertexes_in_facets) {
          polygonIndices[(k + l) + 1] = objDraw->polygons[i].vertexes[0] - 1;
        } else {
          polygonIndices[(k + l) + 1] = objDraw->polygons[i].vertexes[j + 1] - 1;
        }
      }
      k += objDraw->polygons[i].number_of_vertexes_in_facets * 2;
    }
}

//void GLWidget::drawCube(float a){
//    float ver_cub[] = {
//         a,-a,-a,        a,-a, a,       -a,-a, a,       -a, -a, -a,
//         a, a,-a,        a, a, a,       -a, a, a,        -a, a,-a
//    };
//    unsigned int indices[] = {
//        1, 2, 2, 3, 3, 1,
//        7, 6, 6, 5, 5, 7,
//        4, 5, 5, 1, 1, 4,
//        5, 6, 6, 2, 2, 5,
//        2, 6, 6, 7, 7, 2,
//        0, 3, 3, 7, 7, 0,
//        0, 1, 1, 3, 3, 0,
//        4, 7, 7, 5, 5, 4,
//        0, 4, 4, 1, 1, 0,
//        1, 5, 5, 2, 2, 1,
//        3, 2, 2, 7, 7, 3,
//        4, 0, 0, 7, 7, 4
//    };

//    glVertexPointer(3, GL_FLOAT, 0, &ver_cub);
//    glEnableClientState(GL_VERTEX_ARRAY);

//    glDrawElements(GL_LINES, 72 /*size of polygon_indices*/, GL_UNSIGNED_INT, indices);

//    glDisableClientState(GL_VERTEX_ARRAY);
//}
