/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 16:15:10 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 16:16:38 by kuyu             ###   ########.fr       */
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
#define _GNU_SOURCE
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h> // for sleep

volatile sig_atomic_t	g_sig = 0;

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (ft_malloc_error("failed at creating shell", shell), 1);
	init_shell(shell, envp);
	if (shell->interactive && argc == 1)
		interactive_shell(shell);
	else if (argc >= 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		shell->interactive = 0;
		non_interactive_c(shell, argv[2]);
	}
	else
		non_interactive_no_c(shell, argv);
	ft_process_exit(shell, false);
	return (0);
}
