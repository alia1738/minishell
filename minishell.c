/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 05:56:45 by anasr             #+#    #+#             */
/*   Updated: 2022/04/05 12:19:59 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_signals_n_env(char **env, t_parser_info *p)
{
	hide_signal_markers();
	signal(SIGINT, &handle_signals);
	signal(SIGQUIT, SIG_IGN);
	p->env = dup_array(env);
	nested_minishell(p);
}

int	main(int argc, char **argv, char **env)
{
	t_parser_info	p;

	(void)argc;
	(void)argv;
	ft_bzero(&p, sizeof(t_parser_info));
	setup_signals_n_env(env, &p);
	while (1)
	{
		return_p(&p);
		p.input = readline("\033[1;35mbaby shell\033[2;35m> \e[0m");
		if (!p.input)
		{
			free_double_char(p.env);
			exit(p.exit_code);
		}
		if (p.input[0])
			add_history(p.input);
		if (!check_empty_input(p.input))
			continue ;
		if (check_repeated_meta(p.input, &p) == -1)
			continue ;
		save_cmds(p.input, &p);
		free_everything(&p);
		free(p.cmd_path);
	}
}
