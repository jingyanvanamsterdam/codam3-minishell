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
	write(1, "\n", 1);
	g_sig = SIGINT;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
