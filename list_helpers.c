#include "custom_shell.h"

size_t getListLength(const list_t *list)
{
    size_t count = 0;

    while (list)
    {
        list = list->next;
        count++;
    }
    return count;
}

char **convertListToStringArray(list_t *list)
{
    list_t *node = list;
    size_t count = getListLength(list);
    char **strings;
    char *str;

    if (!list || !count)
        return NULL;

    strings = malloc(sizeof(char *) * (count + 1));
    if (!strings)
        return NULL;

    for (size_t i = 0; node; node = node->next, i++)
    {
        str = malloc(stringLength(node->content) + 1);
        if (!str)
        {
            for (size_t j = 0; j < i; j++)
                free(strings[j]);
            free(strings);
            return NULL;
        }

        copyString(str, node->content);
        strings[i] = str;
    }

    strings[count] = NULL;
    return strings;
}

size_t printListContentsWithIndex(const list_t *list)
{
    size_t count = 0;

    while (list)
    {
        printString(convertNumber(list->index, 10, 0));
        printString(": ");
        printString(list->content ? list->content : "(nil)");
        printString("\n");
        list = list->next;
        count++;
    }
    return count;
}

list_t *findNodeWithPrefix(list_t *list, char *prefix, char c)
{
    char *p = NULL;

    while (list)
    {
        p = findPrefix(list->content, prefix);
        if (p && ((c == -1) || (*p == c)))
            return list;
        list = list->next;
    }
    return NULL;
}

ssize_t getNodeIndex(list_t *list, list_t *node)
{
    size_t index = 0;

    while (list)
    {
        if (list == node)
            return index;
        list = list->next;
        index++;
    }
    return -1;
}
