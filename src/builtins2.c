/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 14:51:58 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/05 14:11:32 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(t_parser_info *p)
{
	char	cwd[PATH_MAX];

	p->exit_code = 0;
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("babyshell: pwd");
		p->exit_code = 1;
	}
	else
		printf("%s\n", cwd);
	return (0);
}

static int	echo_index(char **argv)
{
	int	index;
	int	array_index;

	array_index = 1;
	while (argv[array_index])
	{
		if (argv[array_index][0] != '-' || argv[array_index][1] != 'n')
			return (array_index);
		index = 2;
		while (argv[array_index][index])
		{
			if (argv[array_index][index] != 'n')
				return (array_index);
			index++;
		}
		array_index++;
	}
	return (array_index);
}

int	echo(t_parser_info *p, char **argv)
{
	int		i;
	bool	newline_flag;

	i = echo_index(argv);
	newline_flag = true;
	if (i > 1)
		newline_flag = false;
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[++i])
			printf(" ");
	}
	if (newline_flag)
		printf("\n");
	p->exit_code = 0;
	return (0);
}

static void	baby_exit_errors(t_parser_info *p, bool *e_f, int flag, char *cmd)
{
	if (flag == 1)
	{
		p->exit_code = 1;
		*e_f = false;
		printf("babyshell: exit: too many arguments\n");
	}
	else if (flag == 2)
	{
		p->exit_code = 255;
		printf("babyshell: exit: %s: numeric argument required\n", cmd);
	}
}

void	baby_exit(t_parser_info *p, char **cmd)
{
	bool	exit_flag;

	exit_flag = true;
	if (!p->pipes_count)
		printf("exit\n");
	if (!cmd[1])
		p->exit_code = (unsigned char)p->exit_code;
	else
	{
		if (ft_str_isdigit(cmd[1]) == 1 && !cmd[2] && !check_longmax(cmd[1]))
			p->exit_code = (unsigned char)ft_atoi(cmd[1]);
		else if (ft_str_isdigit(cmd[1]) == 1 && cmd[2] && \
		!check_longmax(cmd[1]))
			baby_exit_errors(p, &exit_flag, 1, 0);
		else
			baby_exit_errors(p, &exit_flag, 2, cmd[1]);
	}
	if (exit_flag && !p->pipes_count)
	{
		free_everything(p);
		free_double_char(p->env);
		free(p->cmd_path);
		exit(p->exit_code);
	}
}
