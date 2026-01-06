#include "utils.h"
// for the ERROR defined in parse.h
#include "parse.h"
#include "libft.h"
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h> 

void	ft_malloc_failure(char *s, t_shell *shell)
{
	ft_putstr_fd(ERROR "minishell: " RESET, 2);
	ft_putstr_fd("malloc failure at ", 2);
	ft_putstr_fd(s, 2);
	free_shell(shell);
	exit(EXIT_FAILURE);
}

void	ft_input_error(char *errmes, char *s, t_shell *shell)
{
	ft_putstr_fd(ERROR "minishell: syntax error: " RESET, 2);
	ft_putstr_fd(errmes, 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("\n", 2);
	if (shell->token)
		free_token_lst(&(shell->token));
	if (shell->cmd)
		free_cmd_lst((&shell->cmd));
}

// To do check when the error is being printed if cannot find the file and there is heredoc.
void	ft_error_printing(char *mes)
{
	ft_putstr_fd(ERROR "minishell: " RESET, 2);
	ft_putstr_fd(mes, 2);
	write(2, ": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	write(2, "\n", 1);
}

// sh has no warning. bash has long warning message. decide whether do warning. 
void	ft_warning_printing(void)
{
	ft_putstr_fd(ERROR "minishell: warning: heredoc delimited by end of file\n" RESET, 2);
}
