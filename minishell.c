/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 05:56:45 by anasr             #+#    #+#             */
/*   Updated: 2022/04/02 17:39:19 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	save_input_output_files_n_cmds(int array_index, char **specific_cmd, t_parser_info *p)
{
	int	i;
	int	in_index;
	int	out_index;
	int	cmd_index;
	char	*temp;

	i = 0;
	in_index = 0;
	out_index = 0;
	cmd_index = 0;
	while (specific_cmd[i])
	{
		if ((!ft_strncmp(specific_cmd[i], "<<", 2) || !ft_strncmp(specific_cmd[i], "<", 1)) && specific_cmd[i + 1])
		{
			if (!ft_strncmp(specific_cmd[i], "<<", 2))
				p->in_arrow_flag[array_index][in_index] = DOUBLE_ARROW;
			else
				p->in_arrow_flag[array_index][in_index] = SINGLE_ARROW;
			if (p->in_arrow_flag[array_index][in_index] == SINGLE_ARROW && ft_strchr(specific_cmd[i + 1], '$'))
			{
				temp = expand_dollars_in_str(specific_cmd[i + 1], p, false);
				if (!temp[0])
				{
					printf("babyshell: %s: ambiguous redirect\n", specific_cmd[i + 1]);
					p->in_arrow_flag[array_index][in_index] = -1;
				}
				free(specific_cmd[i + 1]);
				specific_cmd[i + 1] = temp;
			}
			p->input_files_delimiters[array_index][in_index++] = specific_cmd[++i];
		}
		else if ((!ft_strncmp(specific_cmd[i], ">>", 2) || !ft_strncmp(specific_cmd[i], ">", 1)) && specific_cmd[i + 1])
		{
			if (!ft_strncmp(specific_cmd[i], ">>", 2))
				p->out_arrow_flag[array_index][out_index] = DOUBLE_ARROW;
			else
				p->out_arrow_flag[array_index][out_index] = SINGLE_ARROW;

			if (ft_strchr(specific_cmd[i + 1], '$'))
			{
				
				temp = expand_dollars_in_str(specific_cmd[i + 1], p, false);
				if (!temp[0])
				{
					printf("babyshell: %s: ambiguous redirect\n", specific_cmd[i + 1]);
					p->out_arrow_flag[array_index][out_index] = -1;
				}
				free(specific_cmd[i + 1]);
				specific_cmd[i + 1] = temp;
			}
			p->output_files[array_index][out_index++] = specific_cmd[++i];
		}
		else
		{
			if (cmd_index == 0 && ft_strchr(specific_cmd[i], '/'))
				p->cmd_absolute_path[array_index] = true;
			if (ft_strchr(specific_cmd[i], '$'))
			{
				temp = expand_dollars_in_str(specific_cmd[i], p, false);
				free(specific_cmd[i]);
				specific_cmd[i] = temp;
			}
			if (ft_strchr(specific_cmd[i], '\'') || ft_strchr(specific_cmd[i], '\"'))
				specific_cmd[i] = strcpy_wout_quotes(specific_cmd[i]);//u have to free old str
			p->cmd[array_index][cmd_index++] = specific_cmd[i];
		}
		i++;
	}
}

void	save_cmds(char *input, t_parser_info *p)
{
	int		array_index;
	char	*meta[5] = {"<<", "<", ">>", ">", 0};

	p->pipes_count = count_pipes(input);
	allocate_meme_general(p);
	if (p->pipes_count == 0)
	{
		allocate_meme_specific(input, 0, p);
		p->cmd_part[0] = ft_split_custom(input, meta, p);
		save_input_output_files_n_cmds(0, p->cmd_part[0], p);
		p->command_in_action = true;
		execute_single_command(p);
	}
	else
	{
		array_index = 0;
		p->cmd_array = ft_split(input, '|');//account for '|' in quotes
		while (array_index < p->pipes_count + 1)
		{
			allocate_meme_specific(p->cmd_array[array_index], array_index, p);
			p->cmd_part[array_index] = ft_split_custom(p->cmd_array[array_index], meta, p);
			save_input_output_files_n_cmds(array_index, p->cmd_part[array_index], p);
			array_index++;
		}
		p->command_in_action = true;
		execute_pipe_execution(p);
	}
	p->command_in_action = false;
}

t_parser_info	*return_p(t_parser_info *p)
{
	static t_parser_info	*save_p;

	if (p != NULL)
		save_p = p;
	return (save_p);
}

//SIGNAL STUFF
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
		rl_on_new_line();//tells (i think) readline that we moved to a newline
		rl_replace_line("", 1); //replace the rl_buffer (whatever was written (without pressing enter) in readline before signal ctrl c occured) by ""
		if (!p->command_in_action)
			rl_redisplay(); //redisplay prompt and rl_buffer
	}
	return ;
}

void	hide_signal_markers(void)
{
	int		pid;
	char	*ptr[3];
	ptr[0] = "stty";
	ptr[1] = "-echoctl";
	ptr[2] =  0;
	pid = fork();
	if (pid == -1)
		return ;
	else if (pid == 0)
		execve("/bin/stty", ptr, NULL);
	else
		waitpid(-1, NULL, 0);
}

int	check_empty_input(char	*input)
{
	int	i;

	i = 0;
	skip_isspaces(&i, input);
	if (input[i])
		return (1);
	free(input);
	return (0);
}


//
int	main(int argc, char **argv, char **env)
{
	t_parser_info	p;

	(void)argc;
	(void)argv;
	ft_bzero(&p, sizeof(t_parser_info));
	hide_signal_markers();
	rl_catch_signals = 1;
	signal(SIGINT, &handle_signals);
	signal(SIGQUIT, SIG_IGN);
	p.env = dup_array(env);
	nested_minishell(&p);
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
		{
			printf("minishell: syntax error regarding the usage of metacharacters\n");
			free(p.input);
			continue ;
		}
		save_cmds(p.input, &p);
		/*-----------------*/
		free_everything(&p);
		free(p.cmd_path);
	}
}
