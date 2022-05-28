#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

int ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void *ft_memset(void *s, int c, size_t n)
{
	size_t i;
	char *str;

	str = s;
	i = 0;
	while (i < n)
	{
		str[i] = c;
		i++;
	}
	return (s);
}

void *ft_memcpy(void *dst, void *src, size_t n)
{
	size_t i;
	char *d;
	char *s;

	d = dst;
	s = src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

int ft_strncmp(char *s1, char *s2, size_t n)
{
	size_t i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n)
	{
		if (i == n - 1)
			return (0);
		i++;
	}
	return (s1[i] - s2[i]);
}

size_t ft_strlen(char *str)
{
	size_t i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char *ft_strstr(char *str, char *to_find)
{
	size_t i;
	size_t j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (str[i + j] && to_find[j] && str[i + j] == to_find[j])
			j++;
		if (to_find[j] == '\0')
			return (&str[i]);
		i++;
	}
	return (NULL);
}

void print_strs(char **strs)
{
	int i;

	i = 0;
	while (strs[i])
	{
		printf(" - %s\n", strs[i]);
		i++;
	}
}

char *ft_strcpy(char *dst, char *src)
{
	size_t i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
