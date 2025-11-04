/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:06:14 by kuyu              #+#    #+#             */
/*   Updated: 2025/08/21 14:58:48 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

// Need to deal with backward memory free(), if one of following malloc fails

static char	*strdup_betweenmem(char *begin, char *end)
{
	char	*result;
	int		i;

	result = (char *)malloc(sizeof(char) * (end - begin + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < end - begin)
	{
		result[i] = begin[i];
		++i;
	}
	result[i] = '\0';
	return (result);
}

static int	count_substr(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s != '\0')
	{
		while (*s != '\0' && *s == c)
			++s;
		if (*s != '\0' && *s != c)
		{
			++count;
			while (*s != '\0' && *s != c)
				++s;
		}
	}
	return (count);
}

// result is the array of pointers, i is the index at which mem allocation fails
// no need to free result[i], as ith is failure, free until i - 1;
static void	free_malloc(char **result, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free(result[j]);
		++j;
	}
	free(result);
	return ;
}

static char	**split_helper(char **result, char const *s, char c)
{
	char	*begin;
	int		i;

	i = 0;
	while (*s != '\0')
	{
		while (*s != '\0' && *s == c)
			++s;
		if (*s != '\0' && *s != c)
		{
			begin = (char *)s;
			while (*s != '\0' && *s != c)
				++s;
			result[i] = strdup_betweenmem(begin, (char *)s);
			if (!result[i])
			{
				free_malloc(result, i);
				return (NULL);
			}
			++i;
		}
	}
	result[i] = NULL;
	return (result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		count;

	if (!s)
		return (NULL);
	count = count_substr(s, c);
	result = (char **)malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	return (split_helper(result, s, c));
}
