#pragma once

#define DLL_EXPORT extern "C" __declspec(dllexport)

/* Новый интерфейс */

#define NASA_ERROR_SIZE 1024
#define NASA_S_NUM (24 * 60 * 60)
#define NASA_20S_NUM (24 * 60 * 3)
#define NASA_M_NUM (24 * 60 * 1)

// Размер error_message должен быть не менее NASA_ERROR_SIZE
DLL_EXPORT int nasa_s(
        char *error_message,const char *cdf_filename,
        double *t, double *hx, double *hy, double *hz);

DLL_EXPORT int nasa_20s(
        char *error_message,const char *cdf_filename,
        double *t, double *hx, double *hy, double *hz);
DLL_EXPORT int nasa_m(
        char *error_message,const char *cdf_filename,
        double *t, double *hx, double *hy, double *hz);


/* Тестовые */

int nasa(char *error_message, const char *cdf_filename, int length,
        double *t, double *hx, double *hy, double *hz);

//DLL_EXPORT int test_string(char* str, int n);
//DLL_EXPORT int test_array(double **array, int *size);
//DLL_EXPORT double* test_array_2(int *size);
//DLL_EXPORT int dealloc_array(double *array);
//DLL_EXPORT int test_array_3(double *array, int size);
