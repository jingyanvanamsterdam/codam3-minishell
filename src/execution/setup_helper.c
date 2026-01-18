/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 15:39:56 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 15:44:23 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"
#include "env.h"
#include "parse.h"
#include "struct.h"
#include <fcntl.h> // for pid_t
#include <stdlib.h>		// for EXIT_FAILURE
#include <sys/wait.h>
#include <unistd.h>		// for STDIN_FILENO
#include <stdio.h>		// for perror()
#include "libft.h"
#include <errno.h>
#include <string.h>
#define _GNU_SOURCE
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

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
		fd = open(file, O_RDONLY);
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

int	process_input(t_shell *shell)
{
	if (*(shell->input) == '\0' || ft_strcheck(shell->input, ft_isspace))
		return (0);
	add_history(shell->input);
	if (!tokenization(shell))
		return (ft_reset_shell(shell), 0);
	if (!shell->token)
		return (0);
	if (!parsing(shell))
		return (ft_reset_shell(shell), 0);
	free_token_lst(&(shell->token));
	return (1);
}

void	executor(t_shell *shell)
{
	int			count;
	t_builtin	cmd_type;

	if (!shell->cmd || !shell->cmd->cmd[0])
		return ;
	count = count_cmd(shell->cmd);
	cmd_type = is_builtin(shell->cmd->cmd[0]);
	if (count == 1 && cmd_type != OTHERS)
		single_builtin_handler(shell, cmd_type);
	else
	{
		shell->pip_param = malloc(sizeof(t_pipe));
		if (!shell->pip_param)
			return (ft_malloc_error("executuion", shell));
		shell->pip_param->cmd_count = count;
		shell->pip_param->pids = NULL;
		shell->pip_param->pipes = NULL;
		if (count > 1 && !create_pipes(shell))
			return ;
		if (!create_process(shell))
			return ;
		wait_handler(shell);
	}
}
