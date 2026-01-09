#include "parse.h"
#include "struct.h"
#include "libft.h"
#include <stdio.h>	
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h> //for g_sig
#include <sys/errno.h>

//use a static variable i for muiltiple heredoc;
static char	*create_hd_tmp_name(void)
{
	char		*name;
	char		*num;
	static int	i;

	num = NULL;
	num = ft_itoa(i);
	if (!num)
		return (NULL);
	name = ft_strjoin(HD_NAME, num);
	free(num);
	i++;
	return (name);
}

/**
 * malloc failure will exit program, cleaned up everything, close fd.
 * Return -1 or fd. 
 */
//static int		redirect_heredoc(char *res, t_shell *shell)
//{
//	int			p[2];
//	int			ret;
//	char		*name;

//	name = NULL;
//	//if (pipe(p) == -1)
//	//	return (-1); 
//	//ret = write(p[1], res, ft_strlen(res));
//	//if (ret == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
//	//{
//		//name = create_hd_tmp_name();
//		//if (!name)
//		//	ft_malloc_failure("heredoc redirection\n", shell);
//		ret = open(name, O_WRONLY|O_CREAT|O_EXCL|O_TRUNC, 0600);
//		if (ret == -1)
//			return (ret);
//		write(ret, res, ft_strlen(res));
//		close(ret);
//		ret = open(name, O_RDONLY);
//		unlink(name);
//		return (ret);
//	//}
//	//else
//	//	return (p[0]);
//}

bool	is_quoted(char *file)
{
	int	i;

	i = 0;
	while (file[i])
	{
		if (file[i] == '\'' || file[i] == '\"')
			return (true);
		i++;
	}
	return (false);
}

/**
 * bash 5.1: create a pipe buffer to write the heredoc content and read it from.
 * if it is too big, it will create a tmp file in the system
 * Return the read end of pipe or tmp's fd. 
 */
int	heredoc(t_shell *shell, t_redir *redir)
{
	//char	*res;
	bool	quoted;
	char	*delimiter;

	//res = NULL;
	quoted = is_quoted(redir->file);
	delimiter = remove_quote(redir->file, shell, true);
	if (!delimiter)
		return (ft_malloc_error("heredoc\n", shell), -1);
	redir->file = create_hd_tmp_name();
	if (!redir->file)
		return (ft_malloc_error("heredoc name\n", shell), -1);
	if (!do_hd_loop(quoted, delimiter, shell, redir))
		return (-1);
		// should return and exit => check previous function sequence;
	
	redir->fd = open(redir->file, O_RDONLY);
	unlink(redir->file);
	free(delimiter);
	return (1);
} 
