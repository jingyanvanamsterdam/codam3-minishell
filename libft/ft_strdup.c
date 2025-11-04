/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:37:32 by kuyu              #+#    #+#             */
/*   Updated: 2025/04/28 14:27:02 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strcpy(char *dst, const char *src)
{
	char	*dest_origin;

	dest_origin = dst;
	while (*src)
	{
		*dst = *src;
		++dst;
		++src;
	}
	*dst = '\0';
	return (dest_origin);
}

char	*ft_strdup(const char *s)
{
	char	*ptr;

	ptr = (char *)malloc((ft_strlen(s) + 1) * sizeof(const char));
	if (ptr == 0)
		return (0);
	return (ft_strcpy(ptr, s));
}
