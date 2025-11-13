#include "lex.h" //change to minishell.h after combin
#include "struct.h"
#include "libft.h"
#include <stdio.h>	
#include <stdlib.h>
#include <signal.h>
#define _GNU_SOURCE
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

void	handle_sigint(int sig)
{
	(void)sig;
	g_sig = SIGINT;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	//并非async
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_shell *shell;
	
	//Init t_shell 
	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (1);
	shell->env_lst = NULL;
	shell->token = NULL;
	shell->status = GENERAL;
	shell->prev_exit = 0;

	//shell->env_lst = init_env(envp, shell);
	//shell->token = tokenization(argv[1], shell);
	//print_tokens(shell->token);
	//free_shell(shell);

	//setup signal
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		char	*input = readline("Minishell: ");
		if (!input)
		{
			if (g_sig == SIGINT)
			{
				g_sig = 0;
				continue ; 
			}
			// EOF return null; ctrl + d
			write(1, "\n", 1);
			break ; // exit the minishell program?
		}
		if (*input == '\0' || ft_strcheck(input, ft_isspace)) // this do check for tab, but it doesn't work for tab completion in readline.
		{
			free(input);
			continue;
		}
		add_history(input);
		// set up shell
		shell->env_lst = init_env(envp, shell);
		shell->token = tokenization(input, shell);
		print_tokens(shell->token);
		//parse input into token
		//parse token into cmd
		//excusion cmds

		free_env_lst(&(shell->env_lst));
		free_token_list(&(shell->token));
		free(input);
	}
	//print_tokens(head);
	return (0);
}