
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"   // Your real header — keep as is

void add_env(t_shell *shell, char *key, char *value)
{
    // adapt to your env struct
    t_env *new = malloc(sizeof(t_env));
    new->key = strdup(key);
    new->value = value ? strdup(value) : NULL;
    new->next = shell->env;
    shell->env = new;
}

void print_env(t_shell *shell)
{
    t_env *cur = shell->env;
    printf("---- ENV DUMP ----\n");
    while (cur)
    {
        printf("%s=%s\n", cur->key, cur->value ? cur->value : "(null)");
        cur = cur->next;
    }
    printf("------------------\n");
}

void test_echo(t_shell *shell)
{
    printf("\n== Testing echo ==\n");

    char *a1[] = {"echo", "hello", "world", NULL};
    printf("CMD: echo hello world\n");
    ft_echo(a1, shell);

    char *a2[] = {"echo", "-n", "hi", NULL};
    printf("\nCMD: echo -n hi\n");
    ft_echo(a2, shell);

    char *a3[] = {"echo", "-n", "-n", "test", NULL};
    printf("\nCMD: echo -n -n test\n");
    ft_echo(a3, shell);

    char *a4[] = {"echo", "-nabc", "test", NULL};
    printf("\nCMD: echo -nabc test\n");
    ft_echo(a4, shell);

    printf("\nStatus: %d\n", shell->status);
}

void test_pwd(t_shell *shell)
{
    printf("\n== Testing pwd ==\n");

    char *argv[] = {"pwd", NULL};
    int status = ft_pwd(argv, shell);

    printf("\n-- returned status = %d\n", status);
}

void test_export(t_shell *shell)
{
    printf("\n== Testing export ==\n");

    char *a1[] = {"export", "A=1", NULL};
    ft_export(a1, shell);

    char *a2[] = {"export", "B=hello", NULL};
    ft_export(a2, shell);

    char *a3[] = {"export", "A=updated", NULL};
    ft_export(a3, shell);

    print_env(shell);
}

void test_unset(t_shell *shell)
{
    printf("\n== Testing unset ==\n");

    char *u1[] = {"unset", "A", NULL};
    ft_unset(u1, shell);
    print_env(shell);

    char *u2[] = {"unset", "1INVALID", NULL};
    ft_unset(u2, shell);      // Bash: silently ignore invalid
    print_env(shell);
}

void test_env(t_shell *shell)
{
    printf("\n== Testing env ==\n");

    char *e1[] = {"env", NULL};
    ft_env(e1, shell);

    char *e2[] = {"env", "hello", NULL};  // should print error
    ft_env(e2, shell);
}

void test_exit(t_shell *shell)
{
    printf("\n== Testing exit ==\n");

    char *x1[] = {"exit", NULL};
    ft_exit(x1, shell);

    char *x2[] = {"exit", "123", NULL};
    ft_exit(x2, shell);

    char *x3[] = {"exit", "abc", NULL};     // should print error + 255
    ft_exit(x3, shell);

    char *x4[] = {"exit", "1", "2", NULL};  // should NOT exit, return 1
    ft_exit(x4, shell);
}

int main(void)
{
    t_shell shell;
    shell.env = NULL;
    shell.status = 0;

    // Setup initial environment
    add_env(&shell, "PWD", "/home/test");
    add_env(&shell, "OLDPWD", "/home/old");
    add_env(&shell, "HOME", "/home/test");

    printf("\n===== START BUILTIN TESTS =====\n");

    test_echo(&shell);
    test_pwd(&shell);
    test_export(&shell);
    test_unset(&shell);
    test_env(&shell);
    test_exit(&shell);

    printf("===== END TESTS =====\n");
    return 0;
}

