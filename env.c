#include "minishell.h"

int	find_equal(char *env)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	return (i);
}

int	check_env(char *env_variable, t_parser_info *p)
{
	int	i;

	i = 0;
	while (p->env[i])
	{
		if (!ft_strncmp(p->env[i], env_variable, find_equal(p->env[i])))
			return (i);
		i++;
	}
	return (-1);
}

char	**save_env(t_parser_info *p, int *size)
{
	int		i;
	char	**save;

	i = 0;
	while (p->env[i])
		i++;
	save = ft_calloc((i + 1), sizeof(char *));
	if (!save)
		exit(1);
	i = 0;
	while (p->env[i])
	{
		save[i] = p->env[i];
		i++;
	}
	save[i] = 0;
	*size = i;
	return (save);
}

void	new_env(t_parser_info *p, char **save, char *env_variable, int save_index)
{
	int i;

	i = 0;
	if (save_index >= 0)
	{
		while (i != save_index)
			i++;
		if (i == save_index)
			p->env[i] = env_variable;
	}
	if (save_index == -1)
	{
		while (save[i])
		{
			p->env[i] = save[i];
			i++;
		}
		p->env[i++] = env_variable;
		p->env[i] = 0;
	}
}

void	export_env(t_parser_info *p, char *env_variable)
{
	char	**save;
	int		size;
	int		save_index;

	save_index = check_env(env_variable, p);
	save = 0;
	if (save_index >= 0)
	{
		// free(p->env[save_index]);
		// p->env[save_index] = ft_calloc((ft_strlen(env_variable) + 1), sizeof(char));
		new_env(p, save, env_variable, save_index);
	}
	else if (save_index == -1)
	{
		save = save_env(p, &size);
		// free(p->env);
		// p->env = ft_calloc((size + 2), sizeof(char *));
		new_env(p, save, env_variable, save_index);
	}
}

void	dup_array(char **env, t_parser_info *p)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	p->env = ft_calloc((i + 1), sizeof(char *));
	i = 0;
	while (env[i])
	{
		p->env[i] = ft_strdup(env[i]);
		i++;
	}
	p->env[i] = 0;
}

int	main(int argc, char **argv, char **env)
{
	t_parser_info	p;
	int				i;

	(void)argc;
	(void)argv;
	// dup_array(env, &p);
	p.env = env;
	i = 0;
	while (p.env[i])
		printf("%s%s%s\n", YELLOW, p.env[i++], RESET);
	export_env(&p, "HIII=hmmm");
	i = 0;
	while (p.env[i])
		printf("%s%s%s\n", LIGHT_BLUE, p.env[i++], RESET);
}
