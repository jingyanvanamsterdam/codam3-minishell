/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:05:08 by kuyu              #+#    #+#             */
/*   Updated: 2025/04/30 16:22:16 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The  strchr() function returns a pointer to the first occurrence of 
the character c in the string s. 
The strchr() and strrchr() functions return a pointer to the matched character 
or NULL if the character is not found. The terminating null byte is 
considered part of the string, so that if c is specified as '\0', these
functions return a pointer to the terminator.*/
#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == (unsigned char)c)
			return ((char *)s);
		++s;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

/* char *strchr(const char *s, int c)
{
    while (*s)
    {
        if (*s == (char)c)
            return (char *)s;
        s++;
    }
    if ((char)c == '\0') // Handle search for null terminator
        return (char *)s;
    return NULL;
} */

/* int	main()
{
	char *str = "teste";
	printf("%p\n", str);
	printf("%p\n", ft_strchr(str, '\0'));
	return (0);
} */