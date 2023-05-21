#ifndef S21_PARSER_H_
#define S21_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct matrix {
  double **matrix;
  unsigned int rows;
  unsigned int cols;
} matrix_t;

typedef struct facets {
  unsigned int *vertexes;
  unsigned int number_of_vertexes_in_facets;
} polygon_t;

typedef struct data {
  unsigned int count_of_vertexes;  //Кол-во вершин
  unsigned int count_of_facets;    //Кол-во полигонов
  matrix_t matrix_3d;
  polygon_t *polygons;
} data_t;

/**
 * @brief Передача колличества вершин и полигонов
 *
 * @param data Куда записываем результат
 * @param path_to_file Путь к файлу
 */
int s21_parse_amount(data_t *data, char *path_to_file);

/**
 * @brief Заполняет матрицу значениями вершин и добавляет + 1 к началу для
 * правильной индексации
 *
 * @param data Матрица
 * @param path_to_file Путь к файлу
 */
int s21_fill_matrix_with_vertexes(data_t *data, char *path_to_file);

/**
 * @brief Создание матрицы
 *
 * @param rows Строчки
 * @param columns Колонки
 * @param result Куда записываем матрицу
 */
int s21_create_matrix(unsigned int rows, unsigned int columns,
                      matrix_t *result);

/**
 * @brief Простенькая функция для вывода матрицы, созданна для проверки
 *
 * @param matrix Матрица которую надо вывести
 */
void s21_print_matrix(matrix_t *matrix);

/**
 * @brief Простая проверка на валидность матрицы
 *
 * @param A Матрица которую проверяем
 */
int s21_check_matrix(matrix_t *A);

/**
 * @brief Освобождения памяти выделенной для матрицы
 *
 * @param A Матрица
 */
void s21_remove_matrix(matrix_t *A);

#endif  // S21_PARSER_H_