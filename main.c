// Инструменты ввода-вывода
#include <stdio.h>
// Стандартная библиотека
#include <stdlib.h>
// Инструменты для работы с строками
#include <string.h>
// Структура данных "очередь" от BSD
#include <sys/queue.h>
// Тип данных bool
#include <stdbool.h>
// Научные инструменты GSL
#include <gsl/gsl_fit.h>
#include <gsl/gsl_statistics_double.h>
#include <gsl_fit.h>

struct data_point {
    double x;
    double y;
    SLIST_ENTRY(data_point) entries;
};

SLIST_HEAD(data_list, data_point) head = SLIST_HEAD_INITIALIZER(head);

int main() {
    // File and data handling variables
    const char *input_file_name = "data.csv";
    const char *delimiter = "\t";
    const unsigned int skip_header = 3;
    unsigned int column_x = 0;
    unsigned int column_y = 1;
    const char *output_file_name = "fit_C99.csv";
    FILE *input_file = fopen(input_file_name, "r");
    FILE *output_file = fopen(output_file_name, "w"); // Open output file for writing
    if (!input_file || !output_file) {
        fprintf(stderr, "Error opening files.\n");
        return 1;
    }

    printf("#### First Set of Anscombe's Quartet on C99 ####\n");

    char *buffer = NULL;
    size_t buffer_size = 0;
    ssize_t line_length;
    unsigned int row = 0;

    // Skip header lines
    while (row < skip_header && (line_length = getline(&buffer, &buffer_size, input_file)) != -1) {
        row++;
    }

    // Variables for data processing
    double x, y;
    unsigned int data_count = 0;
    SLIST_INIT(&head);

    // Reading data
    while ((line_length = getline(&buffer, &buffer_size, input_file)) != -1) {
        char *token = strtok(buffer, delimiter);
        unsigned int column = 0;
        while (token != NULL) {
            if (column == column_x) {
                x = atof(token);
            } else if (column == column_y) {
                y = atof(token);
            }
            token = strtok(NULL, delimiter);
            column++;
        }
        struct data_point *datum = malloc(sizeof(struct data_point));
        datum->x = x;
        datum->y = y;
        SLIST_INSERT_HEAD(&head, datum, entries);
        data_count++;
    }
    free(buffer); // Free the buffer allocated by getline

    // Preparing data for GSL
    double *xs = malloc(sizeof(double) * data_count);
    double *ys = malloc(sizeof(double) * data_count);
    struct data_point *np;
    unsigned int i = 0;
    SLIST_FOREACH(np, &head, entries) {
        xs[i] = np->x;
        ys[i] = np->y;
        i++;
    }

    // GSL variables for linear fit
    double c0, c1, cov00, cov01, cov11, sumsq;
    gsl_fit_linear(xs, 1, ys, 1, data_count, &c0, &c1, &cov00, &cov01, &cov11, &sumsq);

    // Output results
    printf("Slope: %f\n", c1);
    printf("Intercept: %f\n", c0);

    // Cleanup
    while (!SLIST_EMPTY(&head)) {
        np = SLIST_FIRST(&head);
        SLIST_REMOVE_HEAD(&head, entries);
        free(np);
    }
    free(xs);
    free(ys);
    fclose(input_file);
    fclose(output_file);

    return 0;
}