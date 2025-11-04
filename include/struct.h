#ifndef STRUCT_H
# define STRUCT_H

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND
}			t_token_type;

typedef struct	s_tokens
{
	char			*value;
	t_token_type	type;
	struct s_tokens	*next;
}				t_tokens;

typedef struct	s_cmds
{ 
	//
}				t_cmds;

typedef struct	s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}				t_env;

typedef struct	s_shell
{
	/* data */
}				t_shell;

#endif