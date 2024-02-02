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

// before initialization new big project, solve some common problems on the internet

int main()
{
    const char *input_file_name = "data.csv";
    const char *delimiter = "\t";
    const char *output_file_name = "fit_C99.csv";
    const unsigned int N = 100;
    printf("Hello world!");
    return 0;
}