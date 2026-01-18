/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_hd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 16:24:05 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 16:27:46 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "struct.h"
#include "libft.h"
#include <stdio.h>	
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h> //for g_sig
#include <sys/errno.h>
#include <sys/wait.h>

//use a static variable i for muiltiple heredoc;
static char	*create_hd_tmp_name(void)
{
	char		*name;
	char		*num;
	static int	i;

	num = NULL;
	num = ft_itoa(i);
	if (!num)
		return (NULL);
	name = ft_strjoin(HD_NAME, num);
	free_charptr(&num);
	i++;
	return (name);
}

static bool	is_quoted(char *file)
{
	int	i;

	i = 0;
	while (file[i])
	{
		if (file[i] == '\'' || file[i] == '\"')
			return (true);
		i++;
	}
	return (false);
}

/** return 0 if there is error and fails */
int	do_hd_loop(bool q, char *delimiter, t_shell *shell, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (ft_pipe_error(shell, "fork", 0), 0);
	else if (pid == 0)
	{
		run_hd_process(q, delimiter, shell, cmd);
		close_fd(&(cmd->hdfd));
		ft_process_exit(shell, false);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			shell->exit = 130;
	}
	return (1);
}

/**
 * bash 5.1: create a pipe buffer to write the heredoc content and read it from.
 * if it is too big, it will create a tmp file in the system
 * Return the read end of pipe or tmp's fd. 
 */
int	heredoc(t_shell *shell, t_cmd *cmd, t_redir *redir, char *deli)
{
	bool	quoted;

	quoted = is_quoted(redir->file);
	free_charptr(&(redir->file));
	redir->file = create_hd_tmp_name();
	if (!redir->file)
		return (ft_malloc_error("hd tmp file", shell), 0);
	if (cmd->hdfd != -1)
		close_fd(&(cmd->hdfd));
	cmd->hdfd = open(redir->file, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (cmd->hdfd == -1)
		return (ft_error_printing("heredoc"), 0);
	if (!do_hd_loop(quoted, deli, shell, cmd))
	{
		close_fd(&(cmd->hdfd));
		unlink(redir->file);
		return (0);
	}
	close_fd(&(cmd->hdfd));
	cmd->hdfd = open(redir->file, O_RDONLY);
	unlink(redir->file);
	if (cmd->hdfd == -1)
		return (ft_error_printing("heredoc"), 0);
	return (1);
}

int	handle_cmd_heredoc(t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir;
	char	*delimiter;

	redir = cmd->redir;
	while (redir && redir->type == HEREDOC)
	{
		delimiter = remove_quote(redir->file, shell, true);
		if (!delimiter)
			return (ft_malloc_error("heredoc", shell), 0);
		if (!heredoc(shell, cmd, redir, delimiter))
		{
			free_charptr(&delimiter);
			return (0);
		}
		free_charptr(&(delimiter));
		redir = redir->next;
	}
	return (1);
}
