/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 16:15:43 by anasr             #+#    #+#             */
/*   Updated: 2022/04/03 17:35:03 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int	check_env_unset(char *env_variable, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], env_variable, ft_strlen(env_variable)))
			return (i);
		i++;
	}
	return (-1);
}

char	**remove_var(char **env, char *to_be_removed_var)
{
	int 	i;
	int		new_index;
	char	**new_env;
	int		env_var_index;

	new_index = 0;
	env_var_index = check_env_unset(to_be_removed_var, env);
	if (env_var_index >= 0)
	{
		i = 0;
		while (env[i])
			i++;
		new_env = ft_calloc((i), sizeof(char *));//protect
		i = -1;
		while (env[++i])
		{
			if (i != env_var_index)
				new_env[new_index++] = ft_strdup(env[i]);
		}
		free_double_char(env);
		return (new_env);
	}
	else
		return (env);
}

char	**unset_env(t_parser_info *p, char **env, char *to_be_removed_var)
{
	p->exit_code = 0;
	if (!error_check(p, to_be_removed_var, "unset"))
		return (env);
	if (ft_strchr(to_be_removed_var, '='))
	{
		printf("babyshell: export: '%s': not valid identifier\n", to_be_removed_var);
		p->exit_code = 1;
		return (env);
	}
	if (!ft_strncmp(to_be_removed_var, "OLDPWD", 7))
		p->oldpwd_dont_update = true;
	return (remove_var(env, to_be_removed_var));
}