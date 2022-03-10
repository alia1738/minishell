/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_v2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 15:22:39 by anasr             #+#    #+#             */
/*   Updated: 2022/03/10 16:37:04 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_dollar_content(int *i, char *str)
{
	char	*meta[6] = {"<<", "<", ">>", ">", "|", 0};

	(*i)++;
	while (str[*i] != '$' && ft_ismeta(str[*i], meta) > 0 && ft_isspace(str[*i]) == 0 && str[*i])
		(*i)++;
	if (str[*i] != '$')
		(*i)++;
}

int		len_with_expansion(char	*str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] == '$')
			len += skip_dollar_content(&i, str);
		else if (ft_isquote(str[i]) == 1)
			len += skip_quote_content(&i, str);
		else if (ft_isquote(str[i]) == 2)
	
		else
			i++;
	}
}

char	*expand_dollars_in_str(char *str)
{
	int	i;
	char	
	
	i = 0;
	while (str[i])
	{
		
	}
}

/*********************************/
/*Examples to keep in mind:      */
/*echo "hello$LANG$yeye$USER"$PWD*/
/*echo '$USER'$PWD				 */