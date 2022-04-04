/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 02:25:49 by anasr             #+#    #+#             */
/*   Updated: 2022/04/04 12:16:03 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_char(char **array)
{
	int	i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
	array = 0;
	return ;
}

void	free_triple_char(char ***array)
{
	int	i;

	i = -1;
	while (array[++i])
		free_double_char(array[i]);
	free(array);
	array = 0;
	return ;
}

void	free_triple_char_partial(char ***array, t_parser_info *p)
{
	int	i;

	i = -1;
	while (array[++i] || i < p->pipes_count + 1)
	{
		if (array[i])
			free(array[i]);
	}
	free(array);
	array = 0;
	return ;
}

void	free_double_int(int **array, int rows)
{
	int	i;

	i = -1;
	while (++i < rows)
	{
		if (array[i])
			free(array[i]);
	}
	free(array);
	array = 0;
	return ;
}
