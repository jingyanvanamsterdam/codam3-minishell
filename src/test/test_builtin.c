// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>

// #include "../libft/libft.h"
// #include "../include/minishell.h"
// #include "../include/struct.h"
// #include "../include/builtins.h"

// // -------------------------
// // Helper to create env list
// // -------------------------
// t_env *create_env_node(char *key, char *value)
// {
//     t_env *node = malloc(sizeof(t_env));
//     if (!node)
//         return NULL;

//     node->key = ft_strdup(key);
//     node->value = value ? ft_strdup(value) : NULL;
//     node->next = NULL;
//     return node;
// }

// t_env *init_env_list(char **envp)
// {
//     t_env *head = NULL;
//     t_env *cur = NULL;

//     for (int i = 0; envp[i]; i++)
//     {
//         char **kv = ft_split(envp[i], '=');
//         if (!kv)
//             continue;

//         t_env *new = create_env_node(kv[0], kv[1] ? kv[1] : "");
//         free_2d_arr(kv);

//         if (!head)
//             head = new;
//         else
//             cur->next = new;

//         cur = new;
//     }
//     return head;
// }

// // -------------------------
// // Shell initializer
// // -------------------------
// void init_shell(t_shell *shell, char **envp)
// {
//     shell->env_lst = init_env_list(envp);
//     shell->token = NULL;
//     shell->cmd = NULL;
//     shell->status = 0;
//     shell->prev_exit = 0;
// }

// // -------------------------
// // Utility: print prompt
// // -------------------------
// void print_help()
// {
//     printf("\nAvailable tests:\n");
//     printf(" 1) echo\n");
//     printf(" 2) pwd\n");
//     printf(" 3) env\n");
//     printf(" 4) export\n");
//     printf(" 5) unset\n");
//     printf(" 6) exit\n");
//     printf(" q) quit\n");
// }

// // -------------------------
// // Convert input → argv array
// // -------------------------
// char **str_to_argv(char *input)
// {
//     // remove trailing newline
//     input[strcspn(input, "\n")] = '\0';
//     return ft_split(input, ' ');
// }

// // -------------------------
// // Main test loop
// // -------------------------
// int main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
	
//     t_shell shell;
//     init_shell(&shell, envp);

//     char input[1024];
//     char **args;
//     int choice;

//     printf("=== Minishell Built-in Tester ===\n");
//     print_help();

//     while (1)
//     {
//         printf("\nSelect test: ");
//         fflush(stdout);

//         if (!fgets(input, sizeof(input), stdin))
//             break;

//         if (input[0] == 'q')
//             break;

//         choice = atoi(&input[0]);
//         printf("\n");

//         if (choice == 1) {
//             printf("echo> ");
//             fgets(input, sizeof(input), stdin);
//             args = str_to_argv(input);
//             ft_echo(args, &shell);
//             free_2d_arr(args);
//         }
//         else if (choice == 2) {
//             char *tmp[] = {"pwd", NULL};
//             ft_pwd(tmp, &shell);
//         }
//         else if (choice == 3) {
//             char *tmp[] = {"env", NULL};
//             ft_env(tmp, &shell);
//         }
//         else if (choice == 4) {
//             printf("export> ");
//             fgets(input, sizeof(input), stdin);
//             args = str_to_argv(input);
//             ft_export(args, &shell);
//             free_2d_arr(args);
//         }
//         else if (choice == 5) {
//             printf("unset> ");
//             fgets(input, sizeof(input), stdin);
//             args = str_to_argv(input);
//             ft_unset(args, &shell);
//             free_2d_arr(args);
//         }
//         else if (choice == 6) {
//             printf("exit> ");
//             fgets(input, sizeof(input), stdin);
//             args = str_to_argv(input);
//             ft_exit(args, &shell);
//             free_2d_arr(args);
//         }
//         else {
//             print_help();
//         }
//     }

//     printf("Goodbye!\n");
//     return 0;
// }
