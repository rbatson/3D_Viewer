#include "s21_parser.h"

int s21_parser(data_t *data, char *path_to_file) {
  int err = 0;
  err = s21_parse_amount(data, path_to_file);
  if (!err)
    err = s21_parse_all_data(data, path_to_file);
  return err;
}

int s21_parse_amount(data_t *data, char *path_to_file) {
  int err = 0;
  FILE *file;
  file = fopen(path_to_file, "r");

  if (file != NULL) {
    char *line = NULL;
    size_t len = 0;
    unsigned int amount_of_vertexes = 0;
    unsigned int amount_of_facets = 0;
    while (getline(&line, &len, file) != -1) {
      if (line[0] == 'v' && line[1] == ' ') {
        amount_of_vertexes++;
      } else if (line[0] == 'f') {
        amount_of_facets++;
      }
    }
    data->count_of_vertexes = amount_of_vertexes;
    data->count_of_facets = amount_of_facets;
    free(line);
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
    FILE *file;
    file = fopen(path_to_file, "r");
    if (file != NULL) {
      char *line = NULL;
      size_t len = 0;
      int index = 1;
      while (getline(&line, &len, file) != -1) {
        if (line[0] == 'v' && line[1] == ' ') {
          s21_fill_matrix_with_vertexes(data, &index, line);
        }
      }
      free(line);
      fclose(file);
    }
  } else {
    err = 1;
  }
  return err;
}

void s21_fill_matrix_with_vertexes(data_t *data, int *index, char *line){
  int j = 2;
  for (int i = 0; i < 3; i++) {
    char temp_str[256];
    memset(temp_str, '\0', 256);
    int k = 0;
    while (1) {
      if (line[j] == ' ' || line[j] == '\0') {
        data->matrix_3d.matrix[*index][i] = atof(temp_str);
        break;
      } else {
        temp_str[k] = line[j];
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

int main() {
  data_t *data = malloc(sizeof(data_t));
  char *path_to_file = "test.obj";
  s21_parser(data, path_to_file);
  s21_print_matrix(&data->matrix_3d);
  printf("amount of vertexes = [%d]\n", data->count_of_vertexes);
  printf("amount of facets = [%d]\n", data->count_of_facets);
  s21_remove_matrix(&data->matrix_3d);
  free(data);
}