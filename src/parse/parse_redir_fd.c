#include "parse.h"
#include <fcntl.h>
#include <unistd.h>

int	open_infile(char *file)
{
	int	fd;

	if (access(file, F_OK) == -1 || access(file, R_OK) == -1)
	{
		ft_error_printing(file);
		fd = open("/dev/null", O_RDONLY);
	}
	else
		fd = open(file, O_RDONLY);
	return (fd);
}

int	open_outfile(char *file)
{
	int fd;

	fd = -1;
	if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
		return (fd);
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	return (fd);
}

int	output_append(char *file)
{
	int	fd;

	fd = -1;
	if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
		return (fd);
	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}
