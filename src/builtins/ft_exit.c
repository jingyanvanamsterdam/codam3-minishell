
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
	int	i = 0;

	if (!s || !s[0])
		return (0);
	// Handle optional sign
	if (s[0] == '+' || s[0] == '-')
	{
		++i;
		// Must have at least one digit after sign
		if (!s[i] || !ft_isdigit(s[i]))
			return (0);
	}
	// Check all remaining characters are digits
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
	// Check if entire string was consumed (catches partial parses like "123abc")
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
	// Exit without arguments: exit with status 0 (POSIX compliant)
	if (!argv[1])
		exit(0);
	// Check for non-numeric argument
	if (!parse_exit_code(argv[1], &code))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	// Check for too many arguments (don't exit in this case)
	if (argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->exit = 1;
		return (1);
	}
	// Exit with the code (modulo 256 to ensure 0-255 range)
	exit((unsigned char)code);
}
