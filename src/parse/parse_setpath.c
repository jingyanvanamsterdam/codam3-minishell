/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_setpath.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: kuyu <kuyu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/01/18 16:24:12 by kuyu          #+#    #+#                 */
/*   Updated: 2026/01/18 18:04:01 by jdong         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "struct.h"
#include "parse.h"
#include <unistd.h>
#include "libft.h"

static int	append_slash(char **env_path)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	if (!env_path)
		return (0);
	while (env_path[i])
	{
		tmp = ft_strjoin(env_path[i], "/");
		if (!tmp)
			return (free_2d_arr(env_path), 0);
		free_charptr(&(env_path[i]));
		env_path[i] = tmp;
		tmp = NULL;
		i++;
	}
	return (1);
}

/* Get PATH from enviroment and append / add the end */
char	**create_env_path(t_env *env_lst)
{
	t_env	*head;
	char	**env_path;

	env_path = NULL;
	head = env_lst;
	while (env_lst)
	{
		if (ft_strcmp("PATH", env_lst->key) == 0)
		{
			env_path = ft_split(env_lst->value, ':');
			if (!append_slash(env_path))
				return (NULL);
			break ;
		}
		env_lst = env_lst->next;
	}
	if (head == NULL || env_path == NULL)
	{
		env_path = malloc(sizeof (char *) * 1);
		if (!env_path)
			return (NULL);
		env_path[0] = NULL;
	}
	return (env_path);
}

static char	*set_cmd_path(char *cmd, char **env_paths)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	while (env_paths[i])
	{
		tmp = ft_strjoin(env_paths[i++], cmd);
		if (!tmp)
			return (NULL);
		if (access(tmp, F_OK) == 0)
			break ;
		free(tmp);
		tmp = NULL;
	}
	if (!env_paths[0] || (env_paths[0] != NULL && tmp == NULL))
		return (ft_strdup(""));
	return (tmp);
}

/**return 0 if malloc fails. 
 * handle_redir_fd will return 0 only if heredoc has malloc error,
 * which need to terminate the current paring.
 */
int	set_tcmd_pathes(t_shell *shell)
{
	t_cmd	*cmd;
	char	**env_paths;

	cmd = shell->cmd;
	env_paths = create_env_path(shell->env_lst);
	if (!env_paths)
		return (ft_malloc_error("env path creation", shell), 0);
	while (cmd)
	{
		if (cmd->cmd[0])
		{
			cmd->path = set_cmd_path(cmd->cmd[0], env_paths);
			if (!cmd->path)
			{
				free_2d_arr(env_paths);
				return (ft_malloc_error("path creation", shell), 0);
			}
		}
		cmd = cmd->next;
	}
	free_2d_arr(env_paths);
	return (1);
}
