# Explanation of data structs
## struct
typedef struct	s_shell
{
	t_env			*env_lst;
	t_token			*token;
	t_cmd			*cmd;
	int				prev_exit;
	int				exit;
	// more to add
}				t_shell;
--------------------------------------
typedef struct	s_cmd
{ 
	char			**cmd;
	char			*path;
	t_redir			*redir;
	struct s_cmd	*next;
}				t_cmd;
---------------------------------------
typedef struct	s_redir
{
	t_type			type;
	char			*file;
	int				fd;
	struct s_redir	*next;

}				t_redir;
----------------------------------------
## explain
In t_shell *shell*: 
1. **cmd* is the head node of all the commands 
	- if there are more than one pipes, there will be more commands. aka cmd->next will be how to get to the next command.
2. *exit* is to record the current process's exit code. 
	- After the whole execusion is done, update the prev_exit = exit
3. **env_lst* is a linked list. for execve(), we need a char **enviorn

In t_cmd *cmd*:
1. char **cmd: cmd[0] is the command, the rest is the options of the command.
2. char *path: is the command's execution path, like */usr/bin/echo* for the usage of execve();
3. t_redir *redir:
	1. t_type type: IN, OUT, HEREDOC, APPEND
	2. char *file: the address string of the file directory.
	3. int fd: the fd return by open(file);
	4. next: next redir node. 
	- ATTENTION: 
	During Execution, dup_file, redir need to be checked with one will be replaced with pipes[i][0] and pipes[i][1];

# The functions explain
## pipe.c
1. create_pipes
2. create_process
	- run child process
		- builtin commd
		- execve commd
	- close parent pipes
3. wait for process finish and update shell->exit.

## More functions to work
1. in create_process
	- create_environ()
	- dup_files()
		- redir_fd()_
	- execve_cmd()
	- intergrate builtin functions.
2. wait for process finish
	- set correct exit code. 
3. system error handling
4. signal handling.