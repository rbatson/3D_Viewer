#ifndef AFIN_TRANS
#define AFIN_TRANS

#include "s21_parser.h"

void move_x(data_t* A, double value);
void move_y(data_t* A, double value);
void move_z(data_t* A, double value);

void s21_scale(data_t* A, double scale);
void s21_scale_to_fit(data_t *A);
void s21_center(data_t *A);


void rotation_by_ox(data_t* A, double angle);
void rotation_by_oy(data_t* A, double angle);
void rotation_by_oz(data_t* A, double angle);

#endif
