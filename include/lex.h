#ifndef LEX_H
# define LEX_H

#include "struct.h"

void	free_token_list(t_token *token);
void	free_split(char **key_value);

void	ft_token_failure(char *s, t_token *token);
t_token	*tokenization(char *input, t_token *head);

#endif