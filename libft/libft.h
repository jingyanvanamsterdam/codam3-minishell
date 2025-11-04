/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libft.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: kuyu <kuyu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/25 16:15:55 by ray           #+#    #+#                 */
/*   Updated: 2025/10/28 16:18:21 by jingyandong   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>

char	**ft_split(char const *s, char c);
void	ft_putstr_fd(char *s, int fd);
int		ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, int n);
char	*ft_strjoin(char const *s1, char const *s2);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);
double	ft_atof(char *str);
int		ft_strcmp(const char *s1, const char *s2);

int		ft_isspace(int c);

#endif