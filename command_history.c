#include "custom_shell.h"

char *retrieveHistoryFile(custom_info_t *info)
{
    char *buffer, *directory;

    directory = obtainEnvironmentVariable(info, "HOME=");
    if (!directory)
        return (NULL);
    buffer = malloc(sizeof(char) * (stringLength(directory) + stringLength(HISTORY_FILE) + 2));
    if (!buffer)
        return (NULL);
    buffer[0] = 0;
    copyString(buffer, directory);
    concatenateString(buffer, "/");
    concatenateString(buffer, HISTORY_FILE);
    return (buffer);
}

int saveHistory(custom_info_t *info)
{
    ssize_t fileDescriptor;
    char *filename = retrieveHistoryFile(info);
    list_t *node = NULL;

    if (!filename)
        return (-1);

    fileDescriptor = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(filename);
    if (fileDescriptor == -1)
        return (-1);
    for (node = info->history; node; node = node->next)
    {
        printStringToFileDescriptor(node->str, fileDescriptor);
        putCharToFileDescriptor('\n', fileDescriptor);
    }
    putCharToFileDescriptor(BUFFER_FLUSH, fileDescriptor);
    close(fileDescriptor);
    return (1);
}

int loadHistory(custom_info_t *info)
{
    int i, last = 0, lineCount = 0;
    ssize_t fileDescriptor, readLength, fileSize = 0;
    struct stat fileStat;
    char *buffer = NULL, *filename = retrieveHistoryFile(info);

    if (!filename)
        return (0);

    fileDescriptor = open(filename, O_RDONLY);
    free(filename);
    if (fileDescriptor == -1)
        return (0);
    if (!fstat(fileDescriptor, &fileStat))
        fileSize = fileStat.st_size;
    if (fileSize < 2)
        return (0);
    buffer = malloc(sizeof(char) * (fileSize + 1));
    if (!buffer)
        return (0);
    readLength = read(fileDescriptor, buffer, fileSize);
    buffer[fileSize] = 0;
    if (readLength <= 0)
        return (free(buffer), 0);
    close(fileDescriptor);
    for (i = 0; i < fileSize; i++)
        if (buffer[i] == '\n')
        {
            buffer[i] = 0;
            constructHistoryList(info, buffer + last, lineCount++);
            last = i + 1;
        }
    if (last != i)
        constructHistoryList(info, buffer + last, lineCount++);
    free(buffer);
    info->historyCount = lineCount;
    while (info->historyCount-- >= HISTORY_MAX)
        deleteNodeAtIndex(&(info->history), 0);
    renumberHistory(info);
    return (info->historyCount);
}

int constructHistoryList(custom_info_t *info, char *buffer, int lineCount)
{
    list_t *node = NULL;

    if (info->history)
        node = info->history;
    addNodeEnd(&node, buffer, lineCount);

    if (!info->history)
        info->history = node;
    return (0);
}

int renumberHistory(custom_info_t *info)
{
    list_t *node = info->history;
    int i = 0;

    while (node)
    {
        node->num = i++;
        node = node->next;
    }
    return (info->historyCount = i);
}
