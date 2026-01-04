// #include "env.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// int	main(int argc, char **argv, char **envp)
// {
// 	t_env	*env;
// 	char	*val;
// 	t_shell	*shell;

// 	(void)argc;
// 	(void)argv;

// 	shell = (t_shell *)malloc(sizeof(t_shell));
// 	if (!shell)
// 		return (1);
// 	shell->env_lst = NULL;
// 	shell->token = NULL;
// 	shell->cmd = NULL;
// 	shell->prev_exit = 0;
// 	shell->exit = 0;

// 	/* Initialize env linked list */
// 	init_env(envp, shell);
// 	env = shell->env_lst;
	
// 	printf("=== Original env ===\n");
// 	char **arr = env_to_array(env);
// 	for (int i = 0; arr[i]; i++)
// 	{
// 		printf("%s\n", arr[i]);
// 		free(arr[i]);
// 	}
// 	free(arr);

// 	printf("=== End of original env ===\n");


// 	/* Test env_get_value and env_find */
// 	val = env_get_value(env, "PATH");
// 	if (val)
// 		printf("\nPATH=%s\n", val);
// 	else
// 		printf("\nPATH not found\n");

// 	// TODO: issues from this point

// 	/* Test env_set (add new variable) */
// 	env_set(&env, "TEST_VAR", "hello_world");
// 	val = env_get_value(env, "TEST_VAR");
// 	printf("TEST_VAR=%s\n", val);

// 	/* Test env_set (overwrite existing variable) */
// 	env_set(&env, "TEST_VAR", "new_value");
// 	val = env_get_value(env, "TEST_VAR");
// 	printf("TEST_VAR=%s\n", val);

// 	/* Test env_unset */
// 	env_unset(&env, "TEST_VAR");
// 	val = env_get_value(env, "TEST_VAR");
// 	if (!val)
// 		printf("TEST_VAR successfully unset\n");

// 	/* Free env list */
// 	free_env_list(env);

// 	return (0);
// }
