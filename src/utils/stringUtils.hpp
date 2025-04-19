#pragma once
#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace StringUtils
{
    char **split(const char *str, const char *delimiter, int *count)
    {
        char *s = strdup(str); // sao chép chuỗi gốc
        char *token;
        int size = 10; // kích thước ban đầu
        char **result = (char **)malloc(size * sizeof(char *));
        *count = 0;

        token = strtok(s, delimiter);
        while (token != NULL)
        {
            if (*count >= size)
            {
                size *= 2;
                result = (char **)realloc(result, size * sizeof(char *));
            }
            result[*count] = strdup(token);
            (*count)++;
            token = strtok(NULL, delimiter);
        }

        free(s);
        return result;
    }

    void freeStringArray(char **arr, int count)
    {
        for (int i = 0; i < count; ++i)
        {
            free(arr[i]);
        }
        free(arr);
    }
}

#endif