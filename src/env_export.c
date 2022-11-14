/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 13:56:18 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/04 13:46:52 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**if_new_env_exist(char **env, char *new_env_var, int save_index)
{
	char	*append_var;

	if (append_env_check(new_env_var) == true)
		append_var = ft_strdup(env[save_index]);
	free(env[save_index]);
	if (append_env_check(new_env_var) == true)
	{
		env[save_index] = ft_strjoin(append_var, \
		&new_env_var[find_equal(new_env_var) + 1]);
		free(append_var);
	}
	else
		env[save_index] = ft_strdup(new_env_var);
	return (env);
}

static char	**if_new_env_new(char **env, char *new_env_var)
{
	int		index;
	char	**new_env;

	index = 0;
	while (env[index])
		index++;
	new_env = ft_calloc((index + 2), sizeof(char *));
	index = -1;
	while (env[++index])
		new_env[index] = ft_strdup(env[index]);
	new_env[index] = ft_strdup(new_env_var);
	free_double_char(env);
	return (new_env);
}

char	**export_env(t_parser_info *p, char **env, char *new_env_var)
{
	int		save_index;

	p->exit_code = 0;
	if (!error_check(p, new_env_var, "export"))
		return (env);
	if (!ft_strchr(new_env_var, '='))
		return (env);
	save_index = get_index(new_env_var, env);
	if (save_index >= 0)
		env = if_new_env_exist(env, new_env_var, save_index);
	else if (save_index == -1)
		env = if_new_env_new(env, new_env_var);
	if (!ft_strncmp(new_env_var, "OLDPWD=", 7))
		p->oldpwd_dont_update = false;
	return (env);
}

/* ---------------------------------------------- *\
|*  How To Use:                                   *|
|*    p->env = export_env(p->env, "NAME=value");  *|
\* ---------------------------------------------- */