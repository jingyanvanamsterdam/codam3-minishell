/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 08:35:50 by kuyu              #+#    #+#             */
/*   Updated: 2025/10/07 06:31:11 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

static char	*ft_skip_spaces(char *str)
{
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	return (str);
}

static char	*ft_parse_sign(char *str, int *sign)
{
	*sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			*sign = -1;
		str++;
	}
	return (str);
}

static void	ft_parse_digits(char **str, double *result,
							double *fraction, double *divisor)
{
	bool	after_dot;

	after_dot = false;
	while ((**str >= '0' && **str <= '9') || **str == '.')
	{
		if (**str == '.')
		{
			if (after_dot)
				break ;
			after_dot = true;
			(*str)++;
			continue ;
		}
		if (!after_dot)
			*result = *result * 10.0 + (**str - '0');
		else
		{
			*fraction = *fraction * 10.0 + (**str - '0');
			*divisor *= 10.0;
		}
		(*str)++;
	}
}

double	ft_atof(char *str)
{
	double	result;
	double	fraction;
	double	divisor;
	int		sign;

	str = ft_skip_spaces(str);
	str = ft_parse_sign(str, &sign);
	result = 0.0;
	fraction = 0.0;
	divisor = 1.0;
	ft_parse_digits(&str, &result, &fraction, &divisor);
	return ((result + fraction / divisor) * sign);
}
