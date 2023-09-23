#include "shell.h"

/**
 * customErrorPuts - prints an error string to stderr
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void customErrorPuts(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		customErrorPutchar(str[i]);
		i++;
	}
}

/**
 * customErrorPutchar - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int customErrorPutchar(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * customPutchar - writes the character c to a given file descriptor
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int customPutchar(char c, int fd)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * customPuts - prints an input string to stdout
 * @str: the string to be printed
 *
 * Return: the number of characters printed
 */
int customPuts(char *str)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += customPutchar(*str++, 1);
	}
	return (i);
}
