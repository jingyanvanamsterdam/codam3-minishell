#include "struct.h"
#include "parse.h" //change to minishell.h after combin
#include "libft.h"
#include <stdlib.h> 
#include <string.h>
#include <errno.h>
#include <unistd.h>


void	ft_malloc_failure(char *s, t_shell *shell)
{
	ft_putstr_fd(s, 2);
	free_shell(shell);
	exit(EXIT_FAILURE);
}

void	ft_pipe_error(t_shell *shell, char *str, int **pipes, int n)
{
	ft_error_printing(str);
	free_pipes(pipes, n);
	free_shell(shell);
	exit(EXIT_FAILURE);
}

void	ft_error_printing(char *mes)
{
	ft_putstr_fd("sh: ", 2);
	ft_putstr_fd(mes, 2);
	write(2, ": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	write(2, "\n", 1);
}
