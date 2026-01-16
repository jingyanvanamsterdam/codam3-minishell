#include "minishell.h"
#include "libft.h"

// return 0 if the there is no newline flag, 1 otherwise
static int	is_newline_flag(char *str)
{
	int	i;

	if (str[0] != '-' || !str)
		return (0);
	i = 1;
	while (str[i] == 'n')
		++i;
	if (str[i] == '\0' && i > 1)
		return (1);
	return (0);
}
int	ft_echo(char **argv, t_shell *shell)
{
	int	i;
	int	newline;
	
	(void)shell;
	i = 1;
	newline = 0;
	if (!argv[i])
		return (0);
	while (argv[i] && is_newline_flag(argv[i]))
	{
		newline = 1;
		++i;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putstr_fd(" ", 1);
		++i;
	}
	if (!newline)
		ft_putstr_fd("\n", 1);
	return (0);
}
