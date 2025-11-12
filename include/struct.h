#ifndef STRUCT_H
# define STRUCT_H

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
}			t_token_type;

typedef enum e_lex_stauts
{
	DOUBLE_QUOTE,
	SINGLE_QUOTE,
	GENERAL
}			t_lex_status;

typedef struct	s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}				t_token;

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
	t_env			*env_lst;
	t_token			*token;
	t_lex_status	status;
	int				prev_exit;
	// more to add
}				t_shell;

#endif