/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 13:25:08 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/04 13:28:37 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_equal(char *env)
{
	int	index;

	index = 0;
	while (env[index] && env[index] != '=')
		index++;
	return (index);
}

bool	append_env_check(char *new_env_var)
{
	if (new_env_var[find_equal(new_env_var) - 1] == '+')
		return (true);
	return (false);
}

int	get_index(char *env_variable, char **env)
{
	int	index;
	int	array_index;

	array_index = -1;
	while (env[++array_index])
	{
		index = 0;
		while (env[array_index][index] == env_variable[index] && \
		env[array_index][index] != '=')
			index++;
		if (env[array_index][index] == '=' && ((env_variable[index] == '+' \
		&& env_variable[index + 1] == '=') || env_variable[index] == '='))
			return (array_index);
	}
	return (-1);
}

int	error_check(t_parser_info *p, char *new_var, char *type)
{
	int	index;

	if (!ft_isalpha(new_var[0]) && (new_var[0] != '_'))
	{
		printf("babyshell: %s: '%s': not valid identifier\n", type, new_var);
		p->exit_code = 1;
		return (0);
	}
	index = 1;
	while (new_var[index] && new_var[index] != '=')
	{
		if ((!ft_isalnum(new_var[index]) && new_var[index] != '_' && \
		new_var[index] != '+') || (new_var[index] == '+' && \
		new_var[index + 1] != '='))
		{
			printf("babyshell: %s: '%s': not valid identifier\n", type, new_var);
			p->exit_code = 1;
			return (0);
		}
		index++;
	}
	return (index);
}
