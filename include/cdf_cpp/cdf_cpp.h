#pragma once

#define DLL_EXPORT extern "C" __declspec(dllexport)

/* @func Извлекает данные из .cdf файла и сохраняет в файл
 * назначения.
 * @ret NULL в случае успеха, иначе - описание ошибки.
 */
DLL_EXPORT
const char *
convertor(const char *source, const char *dest);


int nasa(char *error_message, const char *cdf_filename, int length,
        double *t, double *hx, double *hy, double *hz);

/* Новый интерфейс */

#define NASA_ERROR_SIZE 1024
#define NASA_S_NUM (24 * 60 * 60)
#define NASA_20S_NUM (24 * 60 * 3)
#define NASA_M_NUM (24 * 60 * 1)

DLL_EXPORT int nasa_s(
        char *error_message,const char *cdf_filename,
        double *t, double *hx, double *hy, double *hz) {
    return nasa(error_message, cdf_filename, NASA_S_NUM, t, hx, hy, hz);
}

DLL_EXPORT int nasa_20s(
        char *error_message,const char *cdf_filename,
        double *t, double *hx, double *hy, double *hz) {
    return nasa(error_message,cdf_filename,  NASA_20S_NUM, t, hx, hy, hz);
}

DLL_EXPORT int nasa_m(
        char *error_message,const char *cdf_filename,
        double *t, double *hx, double *hy, double *hz) {
    return nasa(error_message, cdf_filename, NASA_M_NUM, t, hx, hy, hz);
}

/* Тестовые */

DLL_EXPORT int test_string(char* str, int n);
DLL_EXPORT int test_array(double **array, int *size);
DLL_EXPORT double* test_array_2(int *size);
DLL_EXPORT int dealloc_array(double *array);
DLL_EXPORT int test_array_3(double *array, int size);

DLL_EXPORT int cdf_get_numvars(int cdf_id, int *numvars);
DLL_EXPORT int cdf_get_var_name(int cdf_id, int varnum, char *var_name);
DLL_EXPORT int cdf_var_get_numrows(int cdf_id, int varnum, int *numrows);
DLL_EXPORT int cdf_var_get_numcolumns(int cdf_id, int varnum, int *numcolumns);
DLL_EXPORT int cdf_var_get_typenumber(int cdf_id, int varnum, int *typenumber);
DLL_EXPORT int cdf_var_get_as_float(int cdf_id, int varnum, int row, int column);
