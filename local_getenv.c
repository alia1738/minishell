/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_getenv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 15:50:10 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/04 14:02:35 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cmp_till_equal(char *p_env, char *var)
{
	int	i;

	i = 0;
	while (p_env[i] != '=')
	{
		if (p_env[i] != var[i])
			return (0);
		i++;
	}
	if (var[i] != 0)
		return (0);
	return (1);
}

char	*local_getenv(char *var, char **p_env)
{
	int	i;

	i = 0;
	while (p_env[i])
	{
		if (cmp_till_equal(p_env[i], var))
			return (&p_env[i][find_equal(p_env[i]) + 1]);
		i++;
	}
	return (0);
}
