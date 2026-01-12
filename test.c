#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int	has_c_option(int ac, char **av)
{
	if (ac == 3 && strcmp(av[1], "-c") == 0)
	{
		printf("%s\n", av[2]);
		return 1;
	}
	else
		return 0;
}

int main(int argc, char **av)
{
	int	interactive = isatty(STDIN_FILENO);
	printf("identify args = %d \n", argc);

	if (argc > 1)
		printf("interactive args = %d\n", interactive);

	if (interactive)
		printf("interactive = %d\n", interactive);
	else
		printf("interative = %d\n", interactive);

	if (has_c_option(argc, av))
		printf("yes");
	int fd = open(av[1], O_RDONLY | O_EXEC);
	printf("fd = %d\n", fd);
	if (fd == -1)
		printf("error: %s\n", strerror(errno));
	if (fd != -1)
		close(fd);
}