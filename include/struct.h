#ifndef STRUCT_H
# define STRUCT_H

typedef enum	e_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	DEFAULT
}				t_type;

typedef enum	e_quote
{
	DOUBLE_QUOTE,
	SINGLE_QUOTE,
	GENERAL
}				t_quote;

typedef enum	e_builtin
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	OTHERS
}				t_builtin;

typedef struct	s_quotok
{
	char 			*value;
	struct s_quotok	*next;
}				t_quotok;

typedef struct	s_redir
{
	t_type			type;
	char			*file;
	struct s_redir	*next;

}				t_redir;

typedef struct	s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}				t_env;

typedef struct	s_token
{
	char			*value;
	t_type			type;
	struct s_token	*next;
}				t_token;

typedef struct	s_cmd
{ 
	char			**cmd;
	char			*path;
	t_redir			*redir;
	int				fd[2];
	int				hdfd;
	struct s_cmd	*next;
}				t_cmd;

typedef struct s_pipe
{
	int	cmd_count;
	int	**pipes;
	int	*pids;
}	t_pipe;

typedef struct	s_shell
{
	char			*input;
	int				interactive;
	t_env			*env_lst;
	t_token			*token;
	t_cmd			*cmd;
	int				prev_exit;
	int				exit;
	t_pipe			*pip_param;
}				t_shell;

#endif