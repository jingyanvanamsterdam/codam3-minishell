#include "lex.h"
#include "struct.h"
#include "libft.h"
#include <stdio.h>	
#include <stdlib.h>

#include <unistd.h> // for sleep

void print_tokens(t_token *head)
{
	printf("======================Start the token:===========================\n");
	while (head)
	{
		if (head->type == WORD)
			printf("%s ", head->value);
		else if (head->type == PIPE)
			printf("| ");
		else if (head->type == REDIR_IN)
			printf("< ");		
		else if (head->type == REDIR_OUT)
			printf("> ");		
		else if (head->type == HEREDOC)
			printf("<< ");
		else if (head->type == APPEND)
			printf(">> ");
		head = head->next;
	}
	printf("\n======================End the token:===========================\n");
}

int main(int argc, char **argv)
{
	t_token	*head;

	head = NULL;
	if (argc == 2)
		head = tokenization(argv[1], head);
	print_tokens(head);
	return (0);
}