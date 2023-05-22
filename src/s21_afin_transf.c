#include "s21_afin_transf.h"

void move_x(data_t* A, double value) {
  for (size_t i = 1; i < A->matrix_3d.rows; i++) {
    A->matrix_3d.matrix[i][0] += value;
  }
}

void move_y(data_t* A, double value) {
  for (size_t i = 1; i < A->matrix_3d.rows; i++) {
    A->matrix_3d.matrix[i][1] += value;
  }
}

void move_z(data_t* A, double value) {
  for (size_t i = 1; i < A->matrix_3d.rows; i++) {
    A->matrix_3d.matrix[i][2] += value;
  }
}

void rotation_by_ox(data_t* A, double angle) {
  for (size_t i = 0; i < A->matrix_3d.rows; i++) {
    double temp_y = A->matrix_3d.matrix[i][1];
    double temp_z = A->matrix_3d.matrix[i][2];
    A->matrix_3d.matrix[i][1] = cos(angle) * temp_y - sin(angle) * temp_z;
    A->matrix_3d.matrix[i][2] = sin(angle) * temp_y + cos(angle) * temp_z;
  }
}

void rotation_by_oy(data_t* A, double angle) {
  for (size_t i = 0; i < A->matrix_3d.rows; i++) {
    double temp_y = A->matrix_3d.matrix[i][1];
    double temp_z = A->matrix_3d.matrix[i][2];
    A->matrix_3d.matrix[i][0] = cos(angle) * temp_y - sin(angle) * temp_z;
    A->matrix_3d.matrix[i][2] = sin(angle) * temp_y + cos(angle) * temp_z;
  }
}

void rotation_by_oz(data_t* A, double angle) {
  for (size_t i = 0; i < A->matrix_3d.rows; i++) {
    double temp_y = A->matrix_3d.matrix[i][1];
    double temp_z = A->matrix_3d.matrix[i][2];
    A->matrix_3d.matrix[i][0] = cos(angle) * temp_y - sin(angle) * temp_z;
    A->matrix_3d.matrix[i][1] = sin(angle) * temp_y + cos(angle) * temp_z;
  }
}

void scaling(data_t *A, double scale) {
  if (scale != 0) {
    for (size_t i = 0; i < A->matrix_3d.rows; i++) {
      A->matrix_3d.matrix[i][0] *= scale;
      A->matrix_3d.matrix[i][1] *= scale;
      A->matrix_3d.matrix[i][2] *= scale;
    }
  }
}