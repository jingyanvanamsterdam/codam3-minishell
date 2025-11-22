#ifndef PIPE_H
# define PIPE_H

# include "struct.h"

// helpers:
int	count_cmd(t_cmd *cmd);
void	init_file(int *file, int **pipes, int i, t_shell *shell);

#endif