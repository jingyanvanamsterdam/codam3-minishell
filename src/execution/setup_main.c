#include "minishell.h"
#include "utils.h"
#include "env.h"
#include "parse.h"
#include "struct.h"
#include "libft.h"
#include <stdio.h>	
#include <stdlib.h>
#include <fcntl.h>

#define _GNU_SOURCE

#include <unistd.h> // for sleep

int	is_script(char **env_paths, char *file, t_shell *shell)
{
	char	*tmp;
	int		i;

	tmp = NULL;
	i = 0;
	if (!env_paths[0] && access(file, F_OK == 0))
		return (ft_putstr_fd("cannot execute binary file", 2), 0);
	else
	{
		while (env_paths[i])
		{
			tmp = ft_strjoin(env_paths[i++], file);
			if (!tmp)
				return (ft_malloc_error("env path creation", shell), 0);
			if (access(tmp, F_OK) == 0)
				break ;
			free(tmp);
			tmp = NULL;
		}
		if (tmp != NULL)
			return (ft_putstr_fd("cannot execute binary file", 2), 0);
	}
	return (1);
}

int	open_argv_fd(char *file, t_shell *shell)
{
	int		fd;
	char	**env_paths;

	env_paths = create_env_path(shell->env_lst);
	if (!env_paths)
		return (ft_malloc_error("env path creation", shell), -1);
	if (is_script(env_paths, file, shell))
	{
		fd = open(file, O_RDONLY | O_EXEC);
		if (fd == -1)
		{
			free_2d_arr(env_paths);
			return (ft_error_printing(file), -1);
		}
		free_2d_arr(env_paths);
		return (fd);
	}
	else
		return (free_2d_arr(env_paths), -1);
}

void	non_interactive_no_c(t_shell *shell, char **av)
{
	int	fd;
	int	read;

	fd = 0;
	if (shell->interactive == 1) // means argc > 1 e.g. ./minishell executable_script
	{
		shell->interactive = 0;
		fd = open_argv_fd(av[1], shell);
		if (fd == -1)
			ft_process_exit(shell, false);
	}
	read = 1; // interactive == 0: e.g. cmd -option | ./minishell or ./minishell < infile
	while (read)
	{
		shell->input = get_next_line(fd);
		if (!shell->input)
			break;
		sig_noninteractive();
		if (!process_input(shell))
			continue;
		executor(shell);
		ft_reset_shell(shell);
	}
	close_fd(&fd);
}

void	non_interactive_c(t_shell *shell, char **av)
{
	char	**inputs;
	int		i;
	char	**key_value;
	t_env	*node;

	i = 2;
	inputs = ft_split(av[2], ';');
	if (!inputs)
		return (ft_malloc_error("paring inputs", shell));
	while (av[++i])
	{
		key_value = ft_split(av[i], '=');
		if (!key_value)
			return (ft_malloc_error("paring inputs", shell));
		node = create_node(key_value, shell);
		if (!node)
			return ;
		append_to_env_lst(&(shell->env_lst), node);
		free_2d_arr(key_value);
	}
	i = -1;
	while (inputs[++i])
	{
		shell->input = inputs[i];
		sig_noninteractive();
		if (!process_input(shell))
			continue;
		executor(shell);
		ft_reset_shell(shell);
	}
	free_2d_arr(inputs);
}
