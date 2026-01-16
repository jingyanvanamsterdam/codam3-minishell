
#include <unistd.h>
#include <stdio.h>
#include "libft.h"
#include "minishell.h"

static char	*get_target_directory(char **argv, t_shell *shell)
{
	char	*target;

    if (!argv[1])
    {
        target = ft_getenv(shell->env_lst, "HOME");
        if (!target)
            ft_builtin_error("cd: ", "HOME", " not set");
        return (target);
    }
    else if (!ft_strcmp(argv[1], "-"))
    {
        target = ft_getenv(shell->env_lst, "OLDPWD");
        if (!target)
            ft_builtin_error("cd: ", "OLDPWD", " not set");
        ft_putstr_fd(target, 1);
        ft_putstr_fd("\n", 1);
        return (target);
    }
    return (argv[1]);
}

static int	update_pwd_variables(t_shell *shell, char *oldpwd)
{
    char	*newpwd;

    update_env_value(shell, "OLDPWD", oldpwd);
    newpwd = getcwd(NULL, 0);
    if (!newpwd)
        return (1);
    update_env_value(shell, "PWD", newpwd);
    free(newpwd);
    return (0);
}

int	ft_cd(char **argv, t_shell *shell)
{
    char	*target;
    char	*oldpwd;

    if (argv[2])
        return (ft_builtin_error("cd: ", "", "too many arguments"), 1);
    target = get_target_directory(argv, shell);
    if (!target)
        return (1);
    oldpwd = getcwd(NULL, 0);
    if (!oldpwd)
        oldpwd = ft_strdup("");
    if (chdir(target) == -1)
    {
        free(oldpwd);
        return (ft_builtin_error("cd: ", target, ": No such file or directory"), 1);
    }
    if (update_pwd_variables(shell, oldpwd) == 1)
    {
        free(oldpwd);
        return (1);
    }
    free(oldpwd);
    return (0);
}
