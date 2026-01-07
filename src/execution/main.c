#include "minishell.h" 
#include "struct.h"
#include "libft.h"
#include <stdio.h>	
#include <stdlib.h>
#include <signal.h>
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

void print_parsed_cmd(t_cmd *head)
{
	int	i;
	
	i = 1;
	while (head)
	{
		printf("======================Start %d of cmd:===========================\n", i);
		int j = 0;
		printf("cmds part: \n");
		while (head->cmd[j])
		{
			printf("%s", head->cmd[j]);
			j++;
			if (head->cmd[j])
				printf(", ");
			else
				printf("\n");
		}
		if (head->redir)
		{
			printf("\nredir parts: \n");
			t_redir *redir = head->redir;
			while (redir)
			{
				printf("redirect type = %d, file is %s, fd = %d\n", redir->type, redir->file, redir->fd);
				redir = redir->next;
			}
		}
		printf("path = %s\n", head->path);
		printf("\n======================End the cmd:===========================\n\n");
		head = head->next;
		i++;
	}
}

void	handle_sigint(int sig)
{
	(void)sig;
	g_sig = SIGINT;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
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

void	ft_restart_safely(t_shell *shell)
{
	shell->prev_exit = shell->exit;
	shell->exit = 0;
	free_cmd_lst(&(shell->cmd));
	//free_pip_params();
	free(shell->input);
	shell->input = NULL;
}

void	init_shell(t_shell *shell, char **envp, int	interactive)
{
	shell->input = NULL;
	shell->interactive = interactive;
	shell->env_lst = NULL;
	shell->token = NULL;
	shell->cmd = NULL;
	shell->pip_param = NULL;
	shell->prev_exit = 0;
	shell->exit = 0;
	init_env(envp, shell);
}
/** return 0 if fails and need to free input and set as NULL; */
int	process_input(t_shell *shell)
{
	// this do check for tab, but it doesn't work for tab completion in readline.
	if (*(shell->input) == '\0' || ft_strcheck(shell->input, ft_isspace))
		return (0);
	add_history(shell->input);
	tokenization(shell);
	if (!shell->token)
		return (0);
	printf("before parings\n");
	parsing(shell);
	if (!shell->cmd)
		return (free_token_lst(&(shell->token)), 0);
	free_token_lst(&(shell->token));
	free(shell->input);
	shell->input = NULL;
	return (1);
}

void	interactive_shell(t_shell *shell)
{
	while (1)
	{
		// set_sig_interactive();
		shell->input = readline("Minishell: ");
		// set_sig_noninteractive();
		//Only when ctrl + d is trigered when there is nothing on the readline
		if (!shell->input) // This is the only exit of this program
		{
			if (g_sig == SIGINT)
			{
				g_sig = 0;
				shell->prev_exit = 130;
				continue;
			}
			// EOF returns null; ctrl+d
			write(1, "exit\n", 6);
			free_shell(shell);
			exit(0);
		}
		if (g_sig == SIGINT)
		{
			g_sig = 0;
			shell->prev_exit = 130;
			free(shell->input);
			shell->input = NULL;
			continue;
		}
		if (!process_input(shell))
		{
			free(shell->input);
			shell->input = NULL;
			continue;
		}
		//executor_tmp(shell);
		print_parsed_cmd(shell->cmd);
		ft_restart_safely(shell);
	}
}

void	noninteractive_shell(char **argv, t_shell *shell)
{
	printf("non interactive %s %p\n", argv[0], shell);
}

int	check_args(int argc, char **argv)
{
	if (argc != 1 && argc != 3)
		return (-1);
	if (argc == 3)
	{
		if (ft_strcmp(argv[1], "-c") != 0)
			return (-1);
		else if (argv[2][0] == '\0' || ft_strcheck(argv[2], ft_isspace))
			return (-1);
		else
			return (0);
	}
	else
		return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell *shell;
	int		interactive;

	interactive = check_args(argc, argv);
	if (interactive == -1)
		ft_shell_input_error();
	//Init t_shell 
	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		ft_malloc_failure("failed at creatint shell", shell);
	init_shell(shell, envp, interactive);
	if (!interactive) // is not interactive
		noninteractive_shell(argv, shell);
	else
		interactive_shell(shell);
	return (0);


	//print_parsed_cmd(shell->cmd);
	//excusion cmds
	//heredoc(shell, test_for_heredoc(shell), 1);
	//char *filename = test_for_outappend(shell);
	//int fd = -1;
	//if (filename)
	//	fd = output_append(filename);
	//write(fd, "hello\n", 6);
	//printf("fd = %d\n", fd);
	//executor(shell);
	//executor_tmp(shell);
	//print_tokens(head);
}
