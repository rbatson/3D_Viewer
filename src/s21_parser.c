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

int main() {
    data_t *data = malloc(sizeof(data_t));
    data->count_of_vertexes = *(unsigned int *)malloc(10 * sizeof(unsigned int));
    data->count_of_facets = *(unsigned int *)malloc(10 * sizeof(unsigned int));
    char *path_to_file = "test.obj";
    parse_amount(data, path_to_file);
    printf("amount of vertexes = [%d]\n", data->count_of_vertexes);
    printf("amount of facets = [%d]\n", data->count_of_facets);
}
