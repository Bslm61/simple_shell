#include "custom_shell.h"

ssize_t bufferInput(info_t *info, char **buffer, size_t *length)
{
    ssize_t bytesRead = 0;
    size_t len_p = 0;

    if (!*length) /* If nothing left in the buffer, fill it */
    {
        /*bfree((void **)info->command_buffer);*/
        free(*buffer);
        *buffer = NULL;
        signal(SIGINT, handleSigint);
#if USE_GETLINE
        bytesRead = getline(buffer, &len_p, stdin);
#else
        bytesRead = customGetline(info, buffer, &len_p);
#endif
        if (bytesRead > 0)
        {
            if ((*buffer)[bytesRead - 1] == '\n')
            {
                (*buffer)[bytesRead - 1] = '\0'; /* Remove trailing newline */
                bytesRead--;
            }
            info->line_count_flag = 1;
            removeComments(*buffer);
            buildHistoryList(info, *buffer, info->history_count++);
            /* if (_strchr(*buffer, ';')) is this a command chain? */
            {
                *length = bytesRead;
                info->command_buffer = buffer;
            }
        }
    }
    return bytesRead;
}

ssize_t getInput(info_t *info)
{
    static char *buffer; /* The ';' command chain buffer */
    static size_t i, j, length;
    ssize_t bytesRead = 0;
    char **buffer_ptr = &(info->argument), *p;

    _putchar(BUF_FLUSH);
    bytesRead = bufferInput(info, &buffer, &length);
    if (bytesRead == -1) /* EOF */
        return -1;
    if (length) /* We have commands left in the chain buffer */
    {
        j = i; /* Init new iterator to current buffer position */
        p = buffer + i; /* Get pointer for return */

        checkChain(info, buffer, &j, i, length);
        while (j < length) /* Iterate to semicolon or end */
        {
            if (isChain(info, buffer, &j))
                break;
            j++;
        }

        i = j + 1; /* Increment past nulled ';'' */
        if (i >= length) /* Reached end of buffer? */
        {
            i = length = 0; /* Reset position and length */
            info->command_buffer_type = CMD_NORM;
        }

        *buffer_ptr = p; /* Pass back pointer to current command position */
        return stringLength(p); /* Return length of current command */
    }

    *buffer_ptr = buffer; /* Else not a chain, pass back buffer from customGetline() */
    return bytesRead; /* Return length of buffer from customGetline() */
}

ssize_t readBuffer(info_t *info, char *buffer, size_t *i)
{
    ssize_t bytesRead = 0;

    if (*i)
        return 0;
    bytesRead = read(info->read_file_descriptor, buffer, READ_BUFFER_SIZE);
    if (bytesRead >= 0)
        *i = bytesRead;
    return bytesRead;
}

int customGetline(info_t *info, char **ptr, size_t *length)
{
    static char buffer[READ_BUFFER_SIZE];
    static size_t i, len;
    size_t k;
    ssize_t bytesRead = 0, s = 0;
    char *p = NULL, *new_p = NULL, *c;

    p = *ptr;
    if (p && length)
        s = *length;
    if (i == len)
        i = len = 0;

    bytesRead = readBuffer(info, buffer, &len);
    if (bytesRead == -1 || (bytesRead == 0 && len == 0))
        return -1;

    c = findCharacter(buffer + i, '\n');
    k = c ? 1 + (unsigned int)(c - buffer) : len;
    new_p = resizeString(p, s, s ? s + k : k + 1);
    if (!new_p) /* MALLOC FAILURE! */
        return (p ? free(p), -1 : -1);

    if (s)
        concatenateStrings(new_p, buffer + i, k - i);
    else
        copySubstring(new_p, buffer + i, k - i + 1);

    s += k - i;
    i = k;
    p = new_p;

    if (length)
        *length = s;
    *ptr = p;
    return s;
}

void handleSigint(__attribute__((unused)) int signal_number)
{
    printString("\n");
    printString("$ ");
    _putchar(BUF_FLUSH);
}
