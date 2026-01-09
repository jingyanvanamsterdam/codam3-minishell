#include "minishell.h"
#include <stdlib.h>
#include "libft.h"
#include <stdio.h>		// for testing purpose

t_env	*create_node(char **key_value, t_shell *shell)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env) * 1);
	if (!node)
		return (ft_malloc_error("env setup", shell), NULL);
	//if (!node)
	//{
	//	free_split(key_value);
	//	ft_malloc_failure("Failture at malloc env.\n", shell);
	//}
	node->key = ft_strdup(key_value[0]);
	if (key_value[1])
		node->value = ft_strdup(key_value[1]);
	else
		node->value = ft_strdup("");
	//if (!node->key || !node->value)
	//{
	//	free_split(key_value);
	//	ft_malloc_failure("Failure at malloc env.\n", shell);
	//}
	if (!node->key || !node->value)
		return (ft_malloc_error("env setup", shell), NULL);
	node->next = NULL;
	return (node);
}

static void	append_to_env_lst(t_env **head, t_env *node)
{
	t_env	*tmp;

	//if (!head || !node)
	//	return ;
	if (*head == NULL)
	{
		*head = node;
		return ;
	}
	tmp = *head;
	while(tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}

int	init_env(char **envp, t_shell *shell)
{
	int		i;
	t_env	*node;		// store the newly created node
	char	**key_value;	// store the splitted result
	
	// TODO: what if envp is empty? 也许可以让ft_split在输入是“”和NULL时都返回NULL，来应对？
	i = 0;
	while (envp[i] != NULL)
	{
		key_value = ft_split(envp[i], '=');
		//if (!key_value)
		//	ft_malloc_failure("setting enviroment values.\n", shell);
		if (!key_value)
			return (ft_malloc_error("setup env key_value", shell), 0);
		node = create_node(key_value, shell);
		if (!node)
			return (0);
		append_to_env_lst(&(shell->env_lst), node);
		free_2d_arr(key_value);
		i++;
	}
	if (envp[0] == NULL)
		printf("no envp\n");
	return (1);
}

// t_env	*init_env(char **envp, t_shell *shell)
// {
// 	int		i;
// 	t_env	*head;
// 	t_env	*node;
// 	t_env	*prev_node;
// 	char	**key_value;

// 	head = NULL;
// 	// TODO: what if envp is empty? 也许可以让ft_split在输入是“”和NULL时都返回NULL，来应对？
// 	i = 0;
// 	while (envp[i] != NULL)
// 	{
// 		key_value = ft_split(envp[i], '=');
// 		if (!key_value)
// 			ft_malloc_failure("Failture at malloc env.\n", shell);
// 		node = create_node(key_value, shell);
// 		if (i != 0)
// 			prev_node->next = node;
// 		else
// 			head = node;
// 		prev_node = node;
// 		free_split(key_value);
// 		i++;
// 	}
// 	return (head);
// }
