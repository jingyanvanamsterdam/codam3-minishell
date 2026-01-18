/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 14:25:20 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 16:37:43 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "struct.h"

//  ======= env_init.c =======
// t_env	*create_node(char **key_value, t_env *env_list);
int		init_env(char **envp, t_shell *shell);

//  ======= env_init_helper.c =======
char	**split_env_string(char *env_str, t_shell *shell);
char	**split_env_with_equals(char *env_str,
			char *equals_pos, t_shell *shell);
char	**split_env_without_equals(char *env_str, t_shell *shell);
int		process_env_entry(char *env_str, t_shell *shell);
void	create_minimal_env(t_shell *shell);

//  ======= env_utils.c =======
char	**env_to_array(t_env *env);

//  ======= env_utils_2.c =======
t_env	*env_find(t_env *env, const char *key);
t_env	*create_env_node_3args(char *key, char *value, t_shell *shell);
void	update_env_value(t_shell *shell, char *key, char *value);
t_env	*create_node(char **key_value, t_shell *shell);
void	append_to_env_lst(t_env **head, t_env *node);

#endif