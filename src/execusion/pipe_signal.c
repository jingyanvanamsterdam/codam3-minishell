#include <signal.h>
//#include "parse.h"
#include "pipe.h"
//#include "struct.h"
//#include "libft.h"
//#include <stdio.h>	
//#include <stdlib.h>
#include <signal.h>
#define _GNU_SOURCE
#include <readline/readline.h>
#include <readline/history.h>
//#include <unistd.h> // for sleep

//用 volatile 保证主程序每次都读最新值。
//赋值可能被打断，如果类型不是 sig_atomic_t，在某些体系结构上大于机器字长的写操作可能会写一半 → 用 sig_atomic_t 保证赋值原子。
volatile sig_atomic_t g_sig = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	//并非async
}

void	handle_sigquit(int sig)
{
	g_sig = SIGQUIT;
	// SIGIGN ignore if it is in the main processs of taking input
	// if it is in the heredoc, it need to print something
}