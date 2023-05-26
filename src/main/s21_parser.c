#include "s21_parser.h"
#include "s21_afin_transf.h"

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
      data->polygons[0].vertexes = NULL;
      while (getline(&str, &len, file) != -1) {
        if (str[0] == 'v' && str[1] == ' ') {
          s21_fill_matrix_with_vertexes(data, &index, str);
        } else if (str[0] == 'f') {
          data->polygons[j].number_of_vertexes_in_facets =
              s21_find_amount_of_vertexes_to_connect(str);
          if (data->polygons[j].number_of_vertexes_in_facets != 0) {
              data->polygons[j].vertexes = calloc(data->polygons[j].number_of_vertexes_in_facets,
                             sizeof(unsigned int));
          } else {
              data->polygons[j].vertexes = NULL;
          }
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
  if (data->polygons[index].vertexes == NULL) {
    return;
  }
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
  if (data == NULL) {
      return;
    }
  for (int i = 0; i < 3; i++) {
    char temp_str[256];
    memset(temp_str, '\0', 256);
    int k = 0;
    while (1) {
      if (str[j] == ' ' || str[j] == '\0') {
          double temp = atof(temp_str);
        data->matrix_3d.matrix[*index][i] = temp;
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
  A->min_max_x = calloc(2, sizeof(double));
  A->min_max_y = calloc(2, sizeof(double));
  A->min_max_z = calloc(2, sizeof(double));
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

void s21_free_data_structure(data_t *data) {
  for (unsigned int i = 0; i < data->matrix_3d.rows; i++) {
    free(data->matrix_3d.matrix[i]);
  }
  for (unsigned int i = 0; i < data->count_of_facets + 1; i++) {
    free(data->polygons[i].vertexes);
  }
  free(data->matrix_3d.matrix);
  free(data->min_max_x);
  free(data->min_max_y);
  free(data->min_max_z);
  free(data->polygons);
  free(data->matrix_conversion_array);
  free(data);
}

double *s21_matrix_into_array(data_t *A) {
  A->matrix_conversion_array = calloc((A->matrix_3d.rows - 1) * A->matrix_3d.cols, sizeof(double));
  for (int i = 0; i < ((A->matrix_3d.rows - 1) * A->matrix_3d.cols); i++) {
    int k = (i + 3) / 3;
    int j = i % 3;
    A->matrix_conversion_array[i] = A->matrix_3d.matrix[k][j];
  }
  return A->matrix_conversion_array;
}

// int main() {
//  data_t *data = calloc(1, sizeof(data_t));
//  char *path_to_file = "../test_obj/test.obj";
//  s21_parser(data, path_to_file);
//  printf("amount of vertexes = [%d]\n", data->count_of_vertexes);
//  printf("amount of facets = [%d]\n", data->count_of_facets);
//  printf("\n");
//  // rotation_by_ox(data, 15);
//  // rotation_by_oy(data, 15);
//  // rotation_by_oz(data, 15);
//  // scaling(data, 1);
//  printf("min and max X = ");
//  printf("[%d]", data->min_max_x[0]);
//  printf("[%d]\n", data->min_max_x[1]);
//  printf("min and max Y = ");
//  printf("[%d]", data->min_max_y[0]);
//  printf("[%d]\n", data->min_max_y[1]);
//  printf("min and max Z = ");
//  printf("[%d]", data->min_max_z[0]);
//  printf("[%d]\n", data->min_max_z[1]);
//  printf("\n");
//  printf("Матрица до центрирования:\n");
//  s21_print_matrix(&data->matrix_3d);
//  printf("\n");
//  s21_scale_to_fit(data);
// //  s21_center(data);
// //  printf("Матрица после центрирования:\n");
// //  s21_print_matrix(&data->matrix_3d);
// //  printf("\n");
//  double *test = s21_matrix_into_array(data);
//  for (int i = 0; i < ((data->matrix_3d.rows - 1) * data->matrix_3d.cols); i++) {
//   printf("[%f]", test[i]);
//  }
//  printf("\n");
//  for (unsigned int i = 1; i < data->count_of_facets + 1; i++) {
//    int j = 0;
//    for (unsigned int k = 0; k < data->polygons[i].number_of_vertexes_in_facets;
//         k++) {
//      printf("'f'[%d][%d] in test.obj = %d\n", i, j,
//             data->polygons[i].vertexes[k]);
//      j++;
//    }
//    printf("\n");
//  }
//  s21_free_data_structure(data);
// }
