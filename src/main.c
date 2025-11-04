#include "minishell.h"
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

void	print_env_list(t_env *head)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (current->key && current->value)
			printf("key: %s, value: %s\n", current->key, current->value);
		current = current->next;
	}
}
//cannot change the readline tab completion
int		input_is_space(char *input)
{
	int	len;
	int	count;

	len = ft_strlen(input);
	count = 0;
	while (*input)
	{
		if (ft_isspace(*input))
			count++;
		input++;
	}
	if (count == len)
		return (1);
	else
		return (0);
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
	t_env	*env_list;
	
	env_list = init_env(envp); // NOTE: env_list can be NULL
	print_env_list(env_list);

	//setup signal
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	//fyi: SIG_IGN是指ignore这个signal，因为ctrl+\ 需要被忽略，如果是child process，kernel收到这个信号会退出child process，我们在wait（）里回收这个信号就可以。所以不用添加hook
	while (1)
	{
		char	*input = readline("Minishell: ");
		if (!input)
		{
			if (g_sig == SIGINT)
			{
				g_sig = 0;
				continue ; 
				// 进入下一轮循环，g_sig在这里是否保留有待研究：后续逻辑中可能判断 “上一次循环是否被中断”？将来计划在某处根据 g_sig 控制逻辑（比如重置状态）
			}
			// EOF return null; ctrl + d
			write(1, "\n", 1);
			break ; // exit the minishell program?
		}
		if (*input == '\0' || input_is_space(input))
		{
			free(input);
			continue;
		}
		add_history(input);
		//parse input into token
		//parse token into cmd
		//excusion cmds
		free(input);
	}
	return (0);
}
