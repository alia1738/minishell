/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 14:27:36 by anasr             #+#    #+#             */
/*   Updated: 2022/04/04 14:30:08 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parser_info	*return_p(t_parser_info *p)
{
	static t_parser_info	*save_p;

	if (p != NULL)
		save_p = p;
	return (save_p);
}

void	handle_signals(int signum)
{
	t_parser_info	*p;

	p = return_p(NULL);
	if (p->command_in_action)
		p->signal_in_cmd = true;
	else
		p->exit_code = 1;
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		if (!p->command_in_action)
			rl_redisplay();
	}
	return ;
}

void	hide_signal_markers(void)
{
	int		pid;
	char	*ptr[3];

	ptr[0] = "stty";
	ptr[1] = "-echoctl";
	ptr[2] = 0;
	pid = fork();
	if (pid == -1)
		return ;
	else if (pid == 0)
		execve("/bin/stty", ptr, NULL);
	else
		waitpid(-1, NULL, 0);
}
