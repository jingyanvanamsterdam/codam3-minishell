#include <signal.h>
//#include "parse.h"
#include "pipe.h"
//#include "struct.h"
//#include "libft.h"
#include <stdio.h>	
#include <stdlib.h>
#define _GNU_SOURCE
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

void	handle_sigint(int sig)
{
	(void)sig;
	g_sig = sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	setup_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGQUIT);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	setup_child_signal(void)
{
	struct sigaction	sa;
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}