/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counting_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 02:49:02 by anasr             #+#    #+#             */
/*   Updated: 2022/03/16 02:55:55 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (count);
}

int	count_cmds_wout_meta(char *str)
{
	int		i;
	int		count;
	char	**temp;
	char	*meta[5] = {"<<", "<", ">>", ">", 0};

	temp = ft_split_custom(str, meta);
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
	return (count);
}
