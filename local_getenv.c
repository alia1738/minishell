/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_getenv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 15:50:10 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/03/15 13:11:17 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	find_equal(char *env)
// {
// 	int	i;

// 	i = 0;
// 	while (env[i] && env[i] != '=')
// 		i++;
// 	return (i);
// }// remove from here after testing cuz this function is already in <env_export.c>

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

char	*local_getenv(char *var, char **p_env) // local_getenv("NAME", p->env)
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

// int	main(int argc, char **argv, char **env)
// {
// 	(void)argc;
// 	(void)argv;
// 	printf("%stest 1:%s%s %s%s\n", GREEN2, RESET, GREEN1, local_getenv("USER", env), RESET);
// 	printf("%stest 2:%s%s %s%s\n", YELLOW2, RESET, YELLOW1, local_getenv("USERR", env), RESET);
// 	printf("%stest 3:%s%s %s%s\n", LIGHT_BLUE2, RESET, LIGHT_BLUE1, local_getenv("PATH", env), RESET);
// 	printf("%stest 4:%s%s %s%s\n", PURPLE2, RESET, PURPLE1, local_getenv("OWO", env), RESET);
// }
