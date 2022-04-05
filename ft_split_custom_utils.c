/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_custom_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 15:16:11 by anasr             #+#    #+#             */
/*   Updated: 2022/04/04 15:23:24 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count_inputs_helper(int *i, char *input, char **meta)
{
	while ((ft_isspace(input[*i]) == 0 && ft_ismeta(&input[*i], meta) == 0 \
	&& input[*i]))
	{
		if (ft_isquote(input[*i]) > 0)
			skip_quote_content(i, input);
		else
			(*i)++;
	}
}
