#include "minishell.h"

int	find_equal(char *env)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	return (i);
}

int	check_env(char *env_variable, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], env_variable, find_equal(env[i])))
			return (i);
		i++;
	}
	return (-1);
}

char	**save_env(char **env, int *size)
{
	int		i;
	char	**save;

	i = 0;
	while (env[i])
		i++;
	save = ft_calloc((i + 1), sizeof(char *));
	if (!save)
		exit(1);
	i = 0;
	while (env[i])
	{
		save[i] = env[i];
		i++;
	}
	save[i] = 0;
	*size = i;
	return (save);
}

void	new_env(char **env, char **save, char *env_variable, int save_index)
{
	int i;

	i = 0;
	if (save_index >= 0)
		env[save_index] = env_variable;
	// {
		// free(env[save_index]);
		// env[save_index] = ft_calloc((ft_strlen(env_variable) + 1), sizeof(char));
	// }
	if (save_index == -1)
	{
		while (save[i])
			i++;
		env[i] = env_variable;
		env[++i] = 0;
	}
}

void	export_env(char	**env, char *env_variable)
{
	char	**save;
	int		size;
	int		save_index;

	save_index = check_env(env_variable, env);
	save = 0;
	if (save_index >= 0)
	{
		new_env(env, save, env_variable, save_index);
	}
	else if (save_index == -1)
	{
		save = save_env(env, &size);
		new_env(env, save, env_variable, save_index);
	}
}

// int	main()
// {
// 	int	i;

// 	i = 0;
// 	// while (environ[i])
// 	// 	printf("%s%s%s\n", YELLOW, environ[i++], RESET);
// 	// export_env(environ, "HIIIIIIIIIIIIIII=:D");
// 	// i = 0;
// 	// while (environ[i])
// 	// 	printf("%s%s%s\n", LIGHT_BLUE, environ[i++], RESET);

// 	/* ---------------------------------------------------- */

// 	pid_t	child;
// 	char	*cmd[3];
// 	cmd[0] = "cd";
// 	cmd[1] =  0;
// 	cmd[2] = 0;

// 	child = fork();
// 	if (!child)
// 	{
// 		export_env(environ, "PWD=:o");
// 		i = 0;
// 		while (environ[i])
// 			printf("%s%s%s\n", MAGENTA, environ[i++], RESET);
// 		execve("builtins/cd", cmd, environ);
// 		printf("HEYEY");
// 	}
// 	else
// 	{
// 		waitpid(-1, 0, 0);
// 		i = 0;
// 		while (environ[i])
// 			printf("%s%s%s\n", GREEN, environ[i++], RESET);
// 	}
// }
