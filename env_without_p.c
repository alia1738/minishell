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
	{
		while (i != save_index)
			i++;
		if (i == save_index)
			env[i] = env_variable;
	}
	if (save_index == -1)
	{
		while (save[i])
		{
			env[i] = save[i];
			i++;
		}
		env[i++] = env_variable;
		env[i] = 0;
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
		new_env(env, save, env_variable, save_index);
	else if (save_index == -1)
	{
		save = save_env(env, &size);
		new_env(env, save, env_variable, save_index);
	}
}


int	main(int argc, char **argv, char **env)
{
	int				i;

	(void)argc;
	(void)argv;
	i = 0;
	while (env[i])
		printf("%s%s%s\n", YELLOW, env[i++], RESET);
	export_env(env, "HI=owwowo");
	i = 0;
	while (env[i])
		printf("%s%s%s\n", LIGHT_BLUE, env[i++], RESET);
	/* ------------------------------------------------- */
	pid_t	child = fork();
	char	*cmd[3];
	cmd[0] = ft_strdup("cd");
	cmd[1] = ft_strdup("dep");
	cmd[2] = 0;
	if (!child)
		execve("/usr/bin/cd", cmd, 0);
	else
		waitpid(-1, 0, 0);
	printf("%s%s%s\n", GREEN, getenv("PWD"), RESET);
	free(cmd[0]);
	free(cmd[1]);
	cmd[0] = ft_strdup("env");
	cmd[1] = 0;
	cmd[2] = 0;
	child = fork();
	if (!child)
		execve("/usr/bin/env", cmd, 0);
	else
		waitpid(-1, 0, 0);
}
