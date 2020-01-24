#pragma once

#define DLL_EXPORT extern "C" __declspec(dllexport)

/**
 * @func Извлекает данные из .cdf файла и сохраняет в файл
 * назначения.
 * @ret NULL в случае успеха, иначе - описание ошибки.
 */
DLL_EXPORT
const char *
convertor(const char *source, const char *dest);

