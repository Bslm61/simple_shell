#include "custom_shell.h"

list_t *prependNode(list_t **list, const char *data, int index)
{
    list_t *newNode;

    if (!list)
        return (NULL);
    newNode = malloc(sizeof(list_t));
    if (!newNode)
        return (NULL);
    initializeList(newNode, index);
    if (data)
    {
        newNode->content = duplicateString(data);
        if (!newNode->content)
        {
            free(newNode);
            return (NULL);
        }
    }
    newNode->next = *list;
    *list = newNode;
    return (newNode);
}

list_t *appendNode(list_t **list, const char *data, int index)
{
    list_t *newNode, *node;

    if (!list)
        return (NULL);

    node = *list;
    newNode = malloc(sizeof(list_t));
    if (!newNode)
        return (NULL);
    initializeList(newNode, index);
    if (data)
    {
        newNode->content = duplicateString(data);
        if (!newNode->content)
        {
            free(newNode);
            return (NULL);
        }
    }
    if (node)
    {
        while (node->next)
            node = node->next;
        node->next = newNode;
    }
    else
        *list = newNode;
    return (newNode);
}

size_t printListContents(const list_t *list)
{
    size_t count = 0;

    while (list)
    {
        printString(list->content ? list->content : "(nil)");
        printString("\n");
        list = list->next;
        count++;
    }
    return (count);
}

int deleteNodeAtIndex(list_t **list, unsigned int index)
{
    list_t *node, *prevNode;
    unsigned int i = 0;

    if (!list || !*list)
        return (0);

    if (!index)
    {
        node = *list;
        *list = (*list)->next;
        free(node->content);
        free(node);
        return (1);
    }
    node = *list;
    while (node)
    {
        if (i == index)
        {
            prevNode->next = node->next;
            free(node->content);
            free(node);
            return (1);
        }
        i++;
        prevNode = node;
        node = node->next;
    }
    return (0);
}

void clearList(list_t **listPtr)
{
    list_t *node, *nextNode, *list;

    if (!listPtr || !*listPtr)
        return;
    list = *listPtr;
    node = list;
    while (node)
    {
        nextNode = node->next;
        free(node->content);
        free(node);
        node = nextNode;
    }
    *listPtr = NULL;
}
