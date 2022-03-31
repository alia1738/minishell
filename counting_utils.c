/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counting_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalsuwai <aalsuwai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 02:49:02 by anasr             #+#    #+#             */
/*   Updated: 2022/03/31 18:56:13 by aalsuwai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(char *input)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (input[++i])
	{
		if (input[i] == '\'')
		{
			while (input[++i] != '\'' && input[i])
				;
		}
		else if (input[i] == '\"')
		{
			while (input[++i] != '\"' && input[i])
				;
		}
		else if (input[i] == '|')
			count++;
		
	}
	return (count);
}

int	count_in_redirections(char	*str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (!ft_strncmp(&str[i], "<<", 2))
		{
			i += 2;
			count++;
		}
		else if (!ft_strncmp(&str[i], "<", 1))
		{
			i++;
			count++;
		}
		else
			i++;
	}
	return (count);
}

int	count_out_redirections(char	*str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (!ft_strncmp(&str[i], ">>", 2))
		{
			i += 2;
			count++;
		}
		else if (!ft_strncmp(&str[i], ">", 1))
		{
			i++;
			count++;
		}
		else
			i++;
	}
	// printf("%s%d%s\n", GREEN1, count, RESET);
	return (count);
}

int	count_cmds_wout_meta(char *str, t_parser_info *p)
{
	int		i;
	int		count;
	char	**temp;
	char	*meta[5] = {"<<", "<", ">>", ">", 0};

	temp = ft_split_custom(str, meta, p);
	i = 0;
	count = 0;
	while (temp[i])
	{
		if (ft_ismeta(temp[i], meta) > 0)
			i++;
		else
			count++;
		i++;
	}
	free_double_char(temp);
	return (count);
}
