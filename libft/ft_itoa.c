/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itoa.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jdong <jdong@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/02 13:55:39 by jdong         #+#    #+#                 */
/*   Updated: 2025/05/06 13:53:00 by jdong         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

static int	count_digits(int n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

static void	convert_int(char *nptr, long number, int sign, int len)
{
	if (sign == -1)
		nptr[0] = '-';
	while (number > 0)
	{
		nptr[--len] = number % 10 + '0';
		number /= 10;
	}
}

char	*ft_itoa(int n)
{
	int		len;
	int		sign;
	long	number;
	char	*nptr;

	sign = 1;
	len = count_digits(n);
	number = n;
	if (n < 0)
	{
		sign = -1;
		number *= -1;
		len++;
	}
	nptr = malloc(len + 1);
	if (!nptr)
		return (NULL);
	nptr[len] = '\0';
	if (n == 0)
		nptr[0] = '0';
	else
		convert_int(nptr, number, sign, len);
	return (nptr);
}
