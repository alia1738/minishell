/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 13:56:18 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/15 13:34:00 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	error_check(char *new_var)
{
	int	i;

	i = 0;
	while (new_var[i] && new_var[i] != '=')
	{
		if (!i && !ft_isalpha(new_var[i]))
		{
			printf("minishell: export: '%s': not valid identifier\n", new_var);
			// change exit code;
			return (0);
		}
		else if (!ft_isalnum(new_var[i]))
		{
			printf("minishell: export: '%s': not valid identifier\n", new_var);
			// change exit code;
			return (0);
		}
		i++;
	}
	return (i);
}

char	**new_env(char **env, char *new_env_var, int save_index)
{
	int i;
	char	**new_env;

	i = 0;
	if (save_index >= 0)
	{
		free(env[save_index]);
		env[save_index] = ft_strdup(new_env_var);
		return (env);
	}
	else if (save_index == -1)
	{
		i = 0;
		while (env[i])
			i++;
		new_env = ft_calloc((i + 2), sizeof(char *));
		i = - 1;
		while (env[++i])
			new_env[i] = ft_strdup(env[i]);
		new_env[i] = ft_strdup(new_env_var);
		free_double(env);
		return (new_env);
	}
	return (0);
}

char	**export_env(char **env, char *new_env_var) // How To Use --> p->env = export_env(p->env, "NAME=value");
{
	int		save_index;

	if (!error_check(new_env_var))
		return (env);
	if (!ft_strchr(new_env_var, '='))
		return (env);
	save_index = check_env(new_env_var, env);
	return (new_env(env, new_env_var, save_index));
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
