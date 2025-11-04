/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:11:01 by kuyu              #+#    #+#             */
/*   Updated: 2025/08/12 09:21:24 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Compare the first n elements. Program has two exits:
One at the end of function, one in the while loop, triggered
by the inequality of two char || reaching the end of 1 or 2 strings*/

int	ft_strncmp(const char *s1, const char *s2, int n)
{
	int	counter;

	counter = 0;
	if (n == 0)
		return (0);
	while (counter < n)
	{
		if ((unsigned char)s1[counter] != (unsigned char)s2[counter]
			|| s1[counter] == '\0')
			return ((unsigned char)s1[counter] - (unsigned char)s2[counter]);
		counter++;
	}
	return (0);
}
// use (unsigned char) cast to make sure when char with ascii value > 127,
// it still correct. e.g. a char with 233 will be interpreted as -23 if 
// char is signed
