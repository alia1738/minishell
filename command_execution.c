/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 18:48:43 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/03 16:58:14 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_n_exit(t_parser_info *p)
{
	free(p->cmd_path);
	free_everything(p);
	free_double_char(p->env);
	free_double_int(p->pipe_append, 1);
	exit(p->exit_code);
}

static void	single_child_process(t_parser_info *p, int pipe_append[2])
{
	if (!p->cmd[0][0] || !builtin_check(p, 0))
		return ;
	account_for_in_redirect(pipe_append, p->in_fd);
	if (p->out_fd > 1)
	{
		dup2(p->out_fd, STDOUT_FILENO);
		close(p->out_fd);
	}
	if (builtin_check(p, 0) == 1)
	{
		builtin_execute(p, 0);
		free_n_exit(p);
	}
	if (p->cmd_path)
		execve(p->cmd_path, p->cmd[0], p->env);
	free_n_exit(p);
}

int	builtin_execute(t_parser_info *p, int i)
{
	if (compare_caseless(p->cmd[i][0], "echo"))
		return (echo(p, p->cmd[i]));
	else if (ft_smartncmp(p->cmd[i][0], "cd", 3))
		return (cd(p, p->cmd[i]));
	else if (compare_caseless(p->cmd[i][0], "pwd"))
		return (pwd(p));
	else if (ft_smartncmp(p->cmd[i][0], "export", 7))
		return (export(p, p->cmd[i]));
	else if (ft_smartncmp(p->cmd[i][0], "unset", 6))
		return (unset(p, p->cmd[i]));
	else if (compare_caseless(p->cmd[i][0], "env"))
		return (env(p));
	else if (ft_smartncmp(p->cmd[i][0], "clear", 6))
		return (clear(p));
	else if (ft_smartncmp(p->cmd[i][0], "exit", 5))
	{
		baby_exit(p, p->cmd[i]);
		return (0);
	}
	return (-1);
}

int	builtin_check(t_parser_info *p, int i)
{
	if (ft_smartncmp(p->cmd[i][0], "cd", 3) || \
	ft_smartncmp(p->cmd[i][0], "export", 7) || \
	ft_smartncmp(p->cmd[i][0], "unset", 6) || \
	ft_smartncmp(p->cmd[i][0], "clear", 6) || \
	ft_smartncmp(p->cmd[i][0], "exit", 5))
		return (0);
	else if (compare_caseless(p->cmd[i][0], "echo") || \
	compare_caseless(p->cmd[i][0], "pwd") || \
	compare_caseless(p->cmd[i][0], "env"))
		return (1);
	return (2);
}

static void	append_n_fds(t_parser_info *p)
{
	close(p->exit_code_fd[0]);
	write(p->exit_code_fd[1], "1", 1);
	do_in_append(p, 0, p->pipe_append[0]);
	signal(SIGINT, SIG_DFL);
	write(p->exit_code_fd[1], "30", 3);
	close(p->exit_code_fd[1]);
	p->in_fd = final_in_fd(0, p);
	if (p->in_fd == -1)
	{
		account_for_in_redirect(p->pipe_append[0], p->in_fd);
		free_everything(p);
		free_double_int(p->pipe_append, 1);
		free_double_char(p->env);
		exit(p->exit_code);
	}
	p->out_fd = final_out_fd(0, p);
	if (p->out_fd == -1)
	{
		account_for_in_redirect(p->pipe_append[0], p->in_fd);
		free_everything(p);
		free_double_int(p->pipe_append, 1);
		free_double_char(p->env);
		exit(p->exit_code);
	}
}

void	before_n_after_single_cmd(t_parser_info *p, bool if_before)
{
	if (if_before == true)
	{
		p->pipe_append = create_pipes(1);
		append_n_fds(p);
		if (builtin_check(p, 0) == 2 && p->cmd_absolute_path[0] == true)
			change_cmd(p, 0);
		else if (builtin_check(p, 0) == 2)
			p->cmd_path = get_cmd_path(p->cmd[0][0], p);
	}
	else
	{
		account_for_in_redirect(p->pipe_append[0], p->in_fd);
		if (p->out_fd > 1)
			close(p->out_fd);
		free_n_exit(p);
	}
}

void	execute_single_command(t_parser_info *p)
{
	int		status;
	char	temp[3];

	ft_bzero(temp, 3);
	pipe(p->exit_code_fd);
	p->child_pids[0] = fork();
	if (!p->child_pids[0])
	{
		before_n_after_single_cmd(p, true);
		single_child_process(p, p->pipe_append[0]);
		before_n_after_single_cmd(p, false);
	}
	else
	{
		if (ft_strrchr(p->cmd[0][0], '/') && !ft_strncmp(ft_strrchr(p->cmd[0][0], '/') + 1, "minishell", 10))
			signal(SIGINT, SIG_IGN);
		waitpid(p->child_pids[0], &status, 0);
		if (ft_strrchr(p->cmd[0][0], '/') && !ft_strncmp(ft_strrchr(p->cmd[0][0], '/') + 1, "minishell", 10))
			signal(SIGINT, handle_signals);
		p->exit_code = WEXITSTATUS(status); //check the logic of getting the exit code
		close(p->exit_code_fd[1]);
		if (p->signal_in_cmd)
		{
			read(p->exit_code_fd[0], temp, 3);
			p->exit_code = ft_atoi(temp);
			p->signal_in_cmd = false;
		}
		close(p->exit_code_fd[0]);
		if (p->cmd[0][0] && !builtin_check(p, 0))
			builtin_execute(p, 0);
	}
}
