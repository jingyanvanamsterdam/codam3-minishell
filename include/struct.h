#ifndef STRUCT_H
# define STRUCT_H

typedef enum e_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
}			t_type;

typedef enum e_lex_stauts
{
	DOUBLE_QUOTE,
	SINGLE_QUOTE,
	GENERAL
}			t_lex_status;

typedef struct	s_token
{
	char			*value;
	t_type			type;
	struct s_token	*next;
}				t_token;

typedef struct	s_redir
{
	t_type			type;
	char			*file;
	struct s_redir	*next;

}				t_redir;


typedef struct	s_cmd
{ 
	char			**cmd;
	char			*path;
	t_redir			*redir;
	struct s_cmd	*next;
}				t_cmd;

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
	t_cmd			*cmd;
	t_lex_status	status;
	int				prev_exit;
	// more to add
}				t_shell;

#endif