#pragma once

#include <stdio.h>
static inline int safe_strcpy(char *dest, const char *src, size_t dest_size) {
    if (!dest || !src || dest_size == 0) 
        return -1; // Invalid arguments

    size_t i = 0;
    while (i < dest_size - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0'; // Always null-terminate

    return (src[i] == '\0') ? 1 : -1; // -1 if truncated
}

static inline int safe_multi_strcat(char *dest, const char *strings[], size_t count, size_t dest_size) {
    if (!dest || !strings || count == 0 || dest_size == 0)
        return -1;

    size_t dest_len = 0;
    while (dest_len < dest_size && dest[dest_len] != '\0')
        dest_len++;

    for (size_t i = 0; i < count; i++) {
        const char *src = strings[i];
        if (!src) continue;

        while (*src != '\0' && dest_len < dest_size - 1)
            dest[dest_len++] = *src++;
    }

    dest[dest_len] = '\0';
    return (dest_len < dest_size) ? 1 : -1;
}

