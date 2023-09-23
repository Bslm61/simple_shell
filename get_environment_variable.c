#include "shell.h"

/**
 * getEnvironmentStrings - returns the string array copy of our environment
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Pointer to the array of environment strings
 */
char **getEnvironmentStrings(info_t *info)
{
	if (!info->environment || info->environmentChanged)
	{
		info->environment = listToStringArray(info->env);
		info->environmentChanged = 0;
	}

	return (info->environment);
}

/**
 * unsetEnvironmentVariable - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @variable: The environment variable to remove
 * Return: 1 on success (delete), 0 otherwise
 */
int unsetEnvironmentVariable(info_t *info, char *variable)
{
	list_t *node = info->env;
	size_t index = 0;
	char *p;

	if (!node || !variable)
		return (0);

	while (node)
	{
		p = startsWith(node->str, variable);
		if (p && *p == '=')
		{
			info->environmentChanged = deleteNodeAtIndex(&(info->env), index);
			index = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		index++;
	}
	return (info->environmentChanged);
}

/**
 * setEnvironmentVariable - Initialize a new environment variable
 *                         or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @variable: The environment variable name
 * @value: The environment variable value
 * Return: Always 0
 */
int setEnvironmentVariable(info_t *info, char *variable, char *value)
{
	char *buffer = NULL;
	list_t *node;
	char *p;

	if (!variable || !value)
		return (0);

	buffer = malloc(_strlen(variable) + _strlen(value) + 2);
	if (!buffer)
		return (1);
	_strcpy(buffer, variable);
	_strcat(buffer, "=");
	_strcat(buffer, value);
	node = info->env;
	while (node)
	{
		p = startsWith(node->str, variable);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buffer;
			info->environmentChanged = 1;
			return (0);
		}
		node = node->next;
	}
	addNodeEnd(&(info->env), buffer, 0);
	free(buffer);
	info->environmentChanged = 1;
	return (0);
}
