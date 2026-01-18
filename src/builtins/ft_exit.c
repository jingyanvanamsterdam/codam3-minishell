/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 14:37:05 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 14:46:49 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <limits.h>

/*  ft_exit() need to be able to exit with different codes.  */
/**
 * Validates if a string is a valid numeric argument for exit.
 * Returns 1 if valid, 0 otherwise.
 * A valid numeric string:
 * - May start with + or -
 * - Must contain at least one digit after the sign
 * - Must contain only digits after the sign
 */
static int	is_valid_numeric(const char *s)
{
	int	i;

	i = 0;
	if (!s || !s[0])
		return (0);
	if (s[0] == '+' || s[0] == '-')
	{
		++i;
		if (!s[i] || !ft_isdigit(s[i]))
			return (0);
	}
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		++i;
	}
	return (1);
}

/**
 * Parses a numeric string to long using strtol.
 * Returns 1 on success, 0 on error (non-numeric or partial parse).
 * Note: Overflow is not checked since exit codes are modulo 256 anyway.
 */
static int	parse_exit_code(const char *s, long *code)
{
	char	*endptr;
	long	result;

	if (!s || !is_valid_numeric(s))
		return (0);
	result = strtol(s, &endptr, 10);
	if (*endptr != '\0')
		return (0);
	*code = result;
	return (1);
}

/**
 * ft_exit - Exit the shell with an optional status code
 * 
 * According to POSIX:
 * - exit without arguments: exit with status 0
 * - exit with numeric argument: exit with that code (modulo 256)
 * - exit with non-numeric argument: exit with status 2
 * - exit with too many arguments: don't exit, return error status 1
 * 
 * @param argv: Command arguments (argv[0] is "exit")
 * @param shell: Shell structure
 * @return: 1 if error (too many arguments), otherwise exits
 */
int	ft_exit(char **argv, t_shell *shell)
{
	long	code;

	ft_putstr_fd("exit\n", 2);
	if (!argv[1])
		exit(0);
	if (!parse_exit_code(argv[1], &code))
	{
		ft_builtin_error("exit: ", argv[1], ": numeric argument required");
		exit(2);
	}
	if (argv[2])
	{
		ft_builtin_error("exit: ", "", "too many arguments");
		shell->exit = 1;
		return (1);
	}
	exit((unsigned char)code);
}
