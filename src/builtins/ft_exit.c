
#include "minishell.h"
#include "libft.h"

static int	is_numeric(const char *s)
{
	int	i = 0;

	if (!s || !s[0])
		return (0);
	if (s[0] == '+' || s[0] == '-')
		++i;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		++i;
	}
	return (1);
}

int	ft_exit(char **argv, t_shell *shell)
{
	long	code;

	ft_putstr_fd("exit\n", 2);
	if (!argv[1])
		exit(shell->prev_exit);				// TODO: check if need to exit with prev_exit or exit?
	if (!is_numeric(argv[1]))
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		exit(2);
	}
	if (argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	code = ft_atoi(argv[1]);
	exit((unsigned char)code);
}
