#ifndef S21_PARSER_H_
#define S21_PARSER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct matrix {
    double **matrix;
    unsigned int rows;
    unsigned int cols;
}matrix_t;

typedef struct facets {
    unsigned int *vertexes;
    unsigned int number_of_vertexes_in_facets;
}polygon_t;

typedef struct data {
    unsigned int count_of_vertexes;  //Кол-во вершин
    unsigned int count_of_facets;    //Кол-во полигонов
    matrix_t martix_3d; 
    polygon_t *polygons;
}data_t;


int parse_amount(data_t *data, char *path_to_file);
int s21_create_matrix(unsigned int rows, unsigned int columns, matrix_t *result);
void s21_print_matrix(matrix_t *matrix);


#endif //S21_PARSER_H_