/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:05:45 by kuyu              #+#    #+#             */
/*   Updated: 2025/04/30 14:54:25 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static char	*ft_join(char const *s1, char const *s2, size_t l1, size_t l2)
{
	char	*str;
	size_t	i;

	str = (char *)malloc(l1 + l2 +1);
	if (!str)
		return (NULL);
	i = 0;
	while (i < l1)
	{
		str[i] = s1[i];
		++i;
	}
	i = 0;
	while (i < l2)
	{
		str[i + l1] = s2[i];
		++i;
	}
	str[l1 + l2] = '\0';
	return (str);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = ft_join(s1, s2, len1, len2);
	return (str);
}
