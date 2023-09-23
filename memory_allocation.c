#include "custom_shell.h"

/**
 * custom_memset - fills memory with a constant byte
 * @s: the pointer to the memory area
 * @b: the byte to fill *s with
 * @n: the amount of bytes to be filled
 * Return: (s) a pointer to the memory area s
 */
char *custom_memset(char *s, char b, unsigned int n)
{
    unsigned int i;

    for (i = 0; i < n; i++)
        s[i] = b;
    return s;
}

/**
 * free_string_array - frees an array of strings
 * @str_array: array of strings to free
 */
void free_string_array(char **str_array)
{
    char **a = str_array;

    if (!str_array)
        return;
    while (*str_array)
        free(*str_array++);
    free(a);
}

/**
 * custom_realloc - reallocates a block of memory
 * @ptr: pointer to the previous malloc'ated block
 * @old_size: byte size of the previous block
 * @new_size: byte size of the new block
 * Return: pointer to the reallocated block or NULL on failure
 */
void *custom_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
    char *p;

    if (!ptr)
        return malloc(new_size);
    if (!new_size)
        return (free(ptr), NULL);
    if (new_size == old_size)
        return ptr;

    p = malloc(new_size);
    if (!p)
        return NULL;

    old_size = old_size < new_size ? old_size : new_size;
    while (old_size--)
        p[old_size] = ((char *)ptr)[old_size];
    free(ptr);
    return p;
}
