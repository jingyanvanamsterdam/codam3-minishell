/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 15:39:58 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 16:41:38 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"
#include "env.h"
#include "parse.h"
#include "struct.h"
#include "libft.h"
#include <stdio.h>	
#include <stdlib.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

void	init_shell(t_shell *shell, char **envp)
{
	bool	interactive_in;
	bool	interactive_out;

	interactive_in = isatty(STDIN_FILENO);
	interactive_out = isatty(STDOUT_FILENO);
	shell->input = NULL;
	if (interactive_in && interactive_out)
		shell->interactive = 1;
	else
		shell->interactive = 0;
	shell->env_lst = NULL;
	shell->token = NULL;
	shell->cmd = NULL;
	shell->pip_param = NULL;
	shell->prev_exit = 0;
	shell->exit = 0;
	if (!init_env(envp, shell))
		ft_process_exit(shell, false);
}

static int	non_interactive_no_c_helper(t_shell *shell, char **av)
{
	int	fd;

	fd = 0;
	if (shell->interactive)
	{
		shell->interactive = 0;
		fd = open_argv_fd(av[1], shell);
		if (fd == -1)
			ft_process_exit(shell, false);
	}
	return (fd);
}

/**
 * interactive == 0: e.g. cmd -option | ./minishell or ./minishell < infile 
 * ./minishell > outfile will not do anything. 
 * shell->interactive == 1 means argc > 1 e.g. ./minishell executable_script*/
void	non_interactive_no_c(t_shell *shell, char **av)
{
	int	fd;
	int	read;

	fd = 0;
	fd = non_interactive_no_c_helper(shell, av);
	if (!isatty(STDOUT_FILENO))
		return ;
	read = 1;
	while (read)
	{
		shell->input = get_next_line(fd);
		if (!shell->input)
			break ;
		sig_noninteractive();
		if (!process_input(shell))
			continue ;
		executor(shell);
		ft_reset_shell(shell);
	}
	close_fd(&fd);
}

void	non_interactive_c(t_shell *shell, char *av)
{
	char	**inputs;
	int		i;

	inputs = ft_split(av, ';');
	if (!inputs)
		return (ft_malloc_error("paring inputs", shell));
	i = -1;
	while (inputs[++i])
	{
		shell->input = ft_strdup(inputs[i]);
		if (!shell->input)
			return (free_2d_arr(inputs),
				ft_malloc_error("set shell inputs", shell));
		sig_noninteractive();
		if (!process_input(shell))
			continue ;
		executor(shell);
		ft_reset_shell(shell);
	}
	free_2d_arr(inputs);
}

/** ctrl + d => EOF, shell->input == NULL, program exit. */
void	interactive_shell(t_shell *shell)
{
	while (1)
	{
		sig_interactive();
		shell->input = readline("Minishell: ");
		sig_noninteractive();
		if (!shell->input)
			ft_process_exit(shell, true);
		if (g_sig == SIGINT)
		{
			g_sig = 0;
			shell->prev_exit = 130;
		}
		if (!process_input(shell))
			continue ;
		executor(shell);
		ft_reset_shell(shell);
	}
}
