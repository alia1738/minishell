#include "../minishell.h"

int	main(int argc, char **argv)
{
	int	i;

	(void)argc;
	i = 1;
	while (argv[i])
		printf("%s ", argv[i++]);
	printf("\n");
	return (0);
}