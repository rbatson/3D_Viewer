#include "s21_parser.h"

int s21_parser(data_t *data, char *path_to_file);

int parse_amount(data_t *data, char *path_to_file){
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

int s21_create_matrix(unsigned int rows, unsigned int columns, matrix_t *result) {
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

void s21_print_matrix(matrix_t *matrix) {
    for (unsigned int i = 0; i < matrix->rows; i++){
        for (unsigned int j = 0; j < matrix->cols; j++) {
            printf("%1.f ", matrix->matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    data_t *data = malloc(sizeof(data_t));
    matrix_t matrix;
    s21_create_matrix(3, 3, &matrix);
    data->count_of_vertexes = *(unsigned int *)malloc(sizeof(unsigned int));
    data->count_of_facets = *(unsigned int *)malloc(sizeof(unsigned int));
    char *path_to_file = "test.obj";
    parse_amount(data, path_to_file);
    s21_print_matrix(&matrix);
    printf("amount of vertexes = [%d]\n", data->count_of_vertexes);
    printf("amount of facets = [%d]\n", data->count_of_facets);
}