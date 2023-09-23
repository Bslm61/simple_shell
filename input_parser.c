#include "custom_shell.h"

/**
 * is_executable - checks if a file is an executable command
 * @info: the information struct
 * @path: the path to the file
 *
 * Return: 1 if it's executable, 0 otherwise
 */
int is_executable(info_t *info, char *path)
{
    struct stat st;

    (void)info;
    if (!path || stat(path, &st))
        return 0;

    if (st.st_mode & S_IFREG)
    {
        return 1;
    }
    return 0;
}

/**
 * duplicate_chars - duplicates characters in a string
 * @path_str: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to a new buffer
 */
char *duplicate_chars(char *path_str, int start, int stop)
{
    static char buf[1024];
    int i = 0, k = 0;

    for (k = 0, i = start; i < stop; i++)
        if (path_str[i] != ':')
            buf[k++] = path_str[i];
    buf[k] = 0;
    return buf;
}

/**
 * find_cmd_in_path - finds a command in the PATH environment variable
 * @info: the information struct
 * @path_str: the PATH string
 * @cmd: the command to find
 *
 * Return: the full path of the command if found, or NULL
 */
char *find_cmd_in_path(info_t *info, char *path_str, char *cmd)
{
    int i = 0, curr_pos = 0;
    char *path;

    if (!path_str)
        return NULL;
    if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
    {
        if (is_executable(info, cmd))
            return cmd;
    }
    while (1)
    {
        if (!path_str[i] || path_str[i] == ':')
        {
            path = duplicate_chars(path_str, curr_pos, i);
            if (!*path)
                _strcat(path, cmd);
            else
            {
                _strcat(path, "/");
                _strcat(path, cmd);
            }
            if (is_executable(info, path))
                return path;
            if (!path_str[i])
                break;
            curr_pos = i;
        }
        i++;
    }
    return NULL;
}
