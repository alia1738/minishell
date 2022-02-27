#include "minishell.h"

int	main()
{
	char	cwd[PATH_MAX];
	if (!getcwd(cwd, sizeof(cwd)))
		perror("minishell: pwd");
	else
		printf("%s\n", cwd);
	return (0);
}