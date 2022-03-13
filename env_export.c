/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 13:56:18 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/13 19:06:49 by aalsuwai         ###   ########.fr       */
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

char	**new_env(char **env, char *new_env_var, int save_index)
{
	int i;
	char	**new_env;

	i = 0;
	if (save_index >= 0)
	{
		while (env[i])
			i++;
		new_env = ft_calloc((i + 1), sizeof(char *));
		i = 0;
		while (env[i])
		{
			if (i == save_index)
				new_env[i] = ft_strdup(new_env_var);
			else
				new_env[i] = ft_strdup(env[i]);
			i++;
		}
		free_double(env);
		return (new_env);
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

char	**export_env(char **env, char *new_env_var) // How To Use --> p->env = export_env(p->env, "NAME=value");
{
	int		save_index;

	if (!error_check(new_env_var))
		return (env);
	if (!ft_strchr(new_env_var, '='))
		return (env);
	save_index = check_env(new_env_var, env);
	if (save_index >= 0)
		return (new_env(env, new_env_var, save_index));
	else if (save_index == -1)
		return (new_env(env, new_env_var, save_index));
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	int		i;
// 	char	**p_env;

// 	(void)argc;
// 	(void)argv;

// 	/* ------------------ Original env ------------------ */
// 	p_env = dup_array(envp);
// 	for (i = 0; p_env[i]; i++)
// 		printf("%s%s%s\n", YELLOW, p_env[i], RESET);

// 	/* -------------------- Test One -------------------- */
// 	p_env = export_env(p_env, "HIIIIIIIIIIIIIII");
// 	for (i = 0; p_env[i]; i++)
// 		printf("%s%s%s\n", LIGHT_BLUE, p_env[i], RESET);
	
// 	/* -------------------- Test Two -------------------- */
// 	p_env = export_env(p_env, "HIIIIIIIIIIIIIII=howowowo");
// 	for (i = 0; p_env[i]; i++)
// 		printf("%s%s%s\n", GREEN, p_env[i], RESET);

// 	/* ------------------- Test Three ------------------- */
// 	p_env = export_env(p_env, "SHELL=");
// 	for (i = 0; p_env[i]; i++)
// 		printf("%s%s%s\n", BLUE, p_env[i], RESET);

// 	/* -------------------- Test Four ------------------- */
// 	p_env = export_env(p_env, "SHELL=Haha");
// 	for (i = 0; p_env[i]; i++)
// 		printf("%s%s%s\n", MAGENTA, p_env[i], RESET);
// }
