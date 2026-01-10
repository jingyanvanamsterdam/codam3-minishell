#define _GNU_SOURCE
//#include "parse.h"
#include "executor.h"
//#include "struct.h"
//#include "libft.h"
#include <stdio.h>	
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <unistd.h>

void	handle_sigint(int sig)
{
	g_sig = sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_new_line(int sig)
{
	g_sig = sig;
	rl_on_new_line();
}
/** addtoset means to ignore - SIGQUIT 
 * sa_flags = 0 => default symplest setting
*/
void	set_sig_interactive(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGQUIT);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/**SIGINT (ctrl -c) and SIGQUIT (ctrl -\) */
void	set_sig_noninteractive(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_new_line;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/**
 * SIG_DFL default signal handler: use default behavior. 
 * ctrl + c will terninate the process.
 */
void	setup_child_signal(void)
{
	struct sigaction	sa;
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}