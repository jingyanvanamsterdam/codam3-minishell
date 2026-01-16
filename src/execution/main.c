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

//用 volatile 保证主程序每次都读最新值。
//赋值可能被打断，如果类型不是 sig_atomic_t，在某些体系结构上大于机器字长的写操作可能会写一半 → 用 sig_atomic_t 保证赋值原子。
volatile sig_atomic_t g_sig = 0;

void print_tokens(t_token *head)
{
	printf("======================Start the token:===========================\n");
	while (head)
	{
		if (head->type == WORD)
			printf("%s, ", head->value);
		else if (head->type == PIPE)
			printf("|, ");
		else if (head->type == REDIR_IN)
			printf("<, ");		
		else if (head->type == REDIR_OUT)
			printf(">, ");		
		else if (head->type == HEREDOC)
			printf("<<, ");
		else if (head->type == APPEND)
			printf(">>, ");
		head = head->next;
	}
	printf("\n======================End the token:===========================\n");
}

t_redir	*test_for_heredoc(t_shell *shell)
{
	t_cmd *cmd = shell->cmd;
	t_redir *redir = NULL;

	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == HEREDOC)
				return redir;
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (redir);
}

char	*test_for_outappend(t_shell *shell)
{
	t_cmd *cmd = shell->cmd;
	t_redir *redir = NULL;

	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == APPEND)
				return redir->file;
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (NULL);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell *shell;

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
