#ifndef AFIN_TRANS
#define AFIN_TRANS

#include "s21_parser.h"

void move_x(data_t* A, double value);
void move_y(data_t* A, double value);
void move_z(data_t* A, double value);

void scaling(data_t* A, double scale);

void rotation_by_ox(data_t* A, double angle);
void rotation_by_oy(data_t* A, double angle);
void rotation_by_oz(data_t* A, double angle);

#endif