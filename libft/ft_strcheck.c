/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcheck.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 17:38:02 by jingyandong       #+#    #+#             */
/*   Updated: 2026/01/18 14:36:25 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcheck(char *s, int (*f)(int))
{
	int	len;
	int	count;

	len = ft_strlen(s);
	count = 0;
	while (*s)
	{
		if (f(*s))
			count++;
		s++;
	}
	if (count == len)
		return (1);
	else
		return (0);
}
