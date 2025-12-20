#include "pipe.h"

void	free_split(char **str_arr)
{
	int	i;

	if (!str_arr)
		return ;
	i = 0;
	while (str_arr[i])
	{
		free(str_arr[i]);
		str_arr[i++] = NULL;
	}
	free(str_arr);
	str_arr = NULL;
}

void	close_pipes(t_pipe *params)
{
	int	i;

	i = 0;
	while (i < params->count - 1)
	{
		close(params->pipes[i][0]);
		close(params->pipes[i][1]);
		++i;
	}
	return ;
}

void	free_pipes(t_pipe *params)
{
	int	i;

	i = 0;
	while (i < params->count - 1)
	{
		free(params->pipes[i]);
		params->pipes[i++] = NULL;
	}
	free(params->pipes);
	params->pipes = NULL;
	return ;
}

// which == 0, open infile, which == 1, open outfile
int	file_opener(char *file, int which, t_pipe *params)
{
	int	fd;

	if (which == 0)
		fd = open(file, O_RDONLY);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(file);
		close_pipes(params);
		free_pipes(params);
		free(params->pids);
		exit(EXIT_FAILURE);
	}
	return (fd);
}
