/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strcheck.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jingyandong <jingyandong@student.codam.      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/29 17:38:02 by jingyandong   #+#    #+#                 */
/*   Updated: 2025/11/04 16:28:45 by jdong         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strcheck(char *s, int (*f)(int))
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
