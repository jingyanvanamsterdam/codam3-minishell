#ifndef UTILS_H
# define UTILS_H

# include "struct.h"

void	ft_input_error(char *errmes, char *s, t_shell *shell);
void	ft_malloc_failure(char *s, t_shell *shell);
void	free_shell(t_shell *shell);


#endif