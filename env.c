#include "minishell.h"

int	find_equal(char *env)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	if (env[i] == '=')
		return (i);
	return (0);
}

int	check_env(char *env_variable, t_parser_info *p)
{
	int	i;

	i = 0;
	while (p->env[i])
	{
		if (ft_strncmp(p->env[i], env_variable, find_equal(p->env[i])))
			return (1);
		i++;
	}
	return (0);
}

void	export_env(t_parser_info *p, char *env_variable)
{
	if (check_env(env_variable, p))
		p->env;
}

