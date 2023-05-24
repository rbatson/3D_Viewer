#include "s21_parser.h"
#include "s21_afin_transf.h"

#include "s21_afin_transf.c"

int s21_parser(data_t *data, char *path_to_file) {
  int err = 0;
  err = s21_parse_amount(data, path_to_file);
  if (!err) err = s21_parse_all_data(data, path_to_file);
  return err;
}

int s21_parse_amount(data_t *data, char *path_to_file) {
  int err = 0;
  FILE *file;
  file = fopen(path_to_file, "r");

  if (file != NULL) {
    char *str = NULL;
    size_t len = 0;
    unsigned int amount_of_vertexes = 0;
    unsigned int amount_of_facets = 0;
    while (getline(&str, &len, file) != -1) {
      if (str[0] == 'v' && str[1] == ' ') {
        amount_of_vertexes++;
      } else if (str[0] == 'f') {
        amount_of_facets++;
      }
    }
    data->count_of_vertexes = amount_of_vertexes;
    data->count_of_facets = amount_of_facets;
    free(str);
    fclose(file);
  } else {
    err = 1;
  }
  return err;
}

int s21_parse_all_data(data_t *data, char *path_to_file) {
  int err = 0;
  matrix_t matrix_of_vertexes;
  err = s21_create_matrix(data->count_of_vertexes + 1, 3, &matrix_of_vertexes);
  if (!err) {
    data->matrix_3d = matrix_of_vertexes;
    data->polygons = malloc((data->count_of_facets + 1) * sizeof(polygon_t));
    FILE *file;
    file = fopen(path_to_file, "r");
    if (file != NULL) {
      char *str = NULL;
      size_t len = 0;
      int index = 1;
      int j = 1;
      while (getline(&str, &len, file) != -1) {
        if (str[0] == 'v' && str[1] == ' ') {
          s21_fill_matrix_with_vertexes(data, &index, str);
        } else if (str[0] == 'f') {
          data->polygons[j].number_of_vertexes_in_facets =
              s21_find_amount_of_vertexes_to_connect(str);
          data->polygons[j].vertexes =
              calloc(data->polygons[j].number_of_vertexes_in_facets,
                     sizeof(unsigned int));
          s21_fill_polygon_data(str, data, j);
          j++;
        }
      }
      s21_find_minmax(data);
      free(str);
      fclose(file);
    }
  } else {
    err = 1;
  }
  return err;
}

unsigned int s21_find_amount_of_vertexes_to_connect(char *str) {
  int i = 0;
  unsigned int count_vertexes = 0;
  for (;;) {
    for (; (str[i] != ' ') && (str[i] != '\0'); i++) {
    }
    if (str[i] == '\0' ||
        (str[i] == ' ' && (str[i + 1] < 48 || str[i + 1] > 57))) {
      break;
    }
    i++;
    count_vertexes++;
  }
  return count_vertexes;
}

void s21_fill_polygon_data(char *str, data_t *data, int index) {
  int i = 0;
  int j = 0;
  while (1) {
    for (; (str[j] != ' ') && (str[j] != '\0'); j++) {
    }
    if (str[j] == '\0') {
      break;
    }
    j++;
    char temp_str[256];
    memset(temp_str, '\0', 256);
    int k = 0;
    for (; str[j] != '/' && str[j] != ' ' && str[j] != '\0'; j++, k++) {
      temp_str[k] = str[j];
    }
    data->polygons[index].vertexes[i] = atoi(temp_str);
    i++;
  }
}

void s21_fill_matrix_with_vertexes(data_t *data, int *index, char *str) {
  int j = 2;
  for (int i = 0; i < 3; i++) {
    char temp_str[256];
    memset(temp_str, '\0', 256);
    int k = 0;
    while (1) {
      if (str[j] == ' ' || str[j] == '\0') {
        data->matrix_3d.matrix[*index][i] = atof(temp_str);
        break;
      } else {
        temp_str[k] = str[j];
        k++;
        j++;
      }
    }
    j++;
  }
  (*index)++;
}

int s21_create_matrix(unsigned int rows, unsigned int columns,
                      matrix_t *result) {
  int res = 0;
  if (rows < 1 || columns < 1)
    res = 1;
  else {
    result->rows = rows;
    result->cols = columns;
    result->matrix = (double **)calloc(rows, sizeof(double *));
    if (result->matrix == NULL) {
      res = 1;
    } else {
      for (unsigned int i = 0; i < rows; i++) {
        result->matrix[i] = (double *)calloc(columns, sizeof(double));
        if (result->matrix[i] == NULL) {
          for (unsigned int j = 0; j < i; j++) {
            free(result->matrix[j]);
          }
          free(result->matrix);
          res = 1;
          break;
        }
      }
    }
  }
  return res;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->matrix) {
    for (unsigned int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->rows = 0;
    A->cols = 0;
    A = NULL;
  }
}

int s21_check_matrix(matrix_t *A) {
  int err = 0;
  if (A == NULL || A->matrix == NULL || A->rows < 1 || A->cols < 1) {
    err = 1;
  }
  return err;
}

void s21_print_matrix(matrix_t *matrix) {
  for (unsigned int i = 0; i < matrix->rows; i++) {
    for (unsigned int j = 0; j < matrix->cols; j++) {
      printf("%1.f ", matrix->matrix[i][j]);
    }
    printf("\n");
  }
}

void s21_find_minmax(data_t *A){
  A->min_max_x = malloc(2 * sizeof(int));
  A->min_max_y = malloc(2 * sizeof(int));
  A->min_max_z = malloc(2 * sizeof(int));
  A->min_max_x[0] = A->matrix_3d.matrix[1][0];
  A->min_max_y[0] = A->matrix_3d.matrix[1][1];
  A->min_max_z[0] = A->matrix_3d.matrix[1][2];
  for (size_t i = 1; i < A->matrix_3d.rows; i++) { 
    if (A->min_max_x[0] > A->matrix_3d.matrix[i][0])
      A->min_max_x[0] = A->matrix_3d.matrix[i][0];
    if (A->min_max_x[1] < A->matrix_3d.matrix[i][0])
      A->min_max_x[1] = A->matrix_3d.matrix[i][0];
    if (A->min_max_y[0] > A->matrix_3d.matrix[i][1]) 
      A->min_max_y[0] = A->matrix_3d.matrix[i][1];
    if (A->min_max_y[1] < A->matrix_3d.matrix[i][1]) 
      A->min_max_y[1] = A->matrix_3d.matrix[i][1];
    if (A->min_max_z[0] > A->matrix_3d.matrix[i][2]) 
      A->min_max_z[0] = A->matrix_3d.matrix[i][2];
    if (A->min_max_z[1] < A->matrix_3d.matrix[i][2]) 
      A->min_max_z[1] = A->matrix_3d.matrix[i][2];
  }
}

int main() {
 data_t *data = malloc(sizeof(data_t));
 char *path_to_file = "../test_obj/test.obj";
 s21_parser(data, path_to_file);
 printf("amount of vertexes = [%d]\n", data->count_of_vertexes);
 printf("amount of facets = [%d]\n", data->count_of_facets);
 printf("\n");
 // rotation_by_ox(data, 15);
 // rotation_by_oy(data, 15);
 // rotation_by_oz(data, 15);
 // scaling(data, 1);
 printf("min and max X = ");
 printf("[%d]", data->min_max_x[0]);
 printf("[%d]\n", data->min_max_x[1]);
 printf("min and max Y = ");
 printf("[%d]", data->min_max_y[0]);
 printf("[%d]\n", data->min_max_y[1]);
 printf("min and max Z = ");
 printf("[%d]", data->min_max_z[0]);
 printf("[%d]\n", data->min_max_z[1]);
 printf("\n");
 printf("Матрица до центрирования:\n");
 s21_print_matrix(&data->matrix_3d);
 printf("\n");
 s21_center(data);
 printf("Матрица после центрирования:\n");
 s21_print_matrix(&data->matrix_3d);
 printf("\n");
 for (unsigned int i = 1; i < data->count_of_facets + 1; i++) {
   int j = 0;
   for (unsigned int k = 0; k < data->polygons[i].number_of_vertexes_in_facets;
        k++) {
     printf("'f'[%d][%d] in test.obj = %d\n", i, j,
            data->polygons[i].vertexes[k]);
     j++;
   }
   printf("\n");
 }
 free(data);
}
