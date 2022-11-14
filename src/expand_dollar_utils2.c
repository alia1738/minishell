/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 12:32:17 by anasr             #+#    #+#             */
/*   Updated: 2022/04/04 13:31:05 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_dollar_question(t_dollar_expansion *dx, t_parser_info *p)
{
	char	*temp;

	temp = ft_itoa(p->exit_code);
	ft_strcpy(&dx->expanded[dx->new_index], temp);
	dx->new_index += ft_strlen(temp);
	free(temp);
	dx->i += 2;
}

void	expand_dollar_general(char *str, t_dollar_expansion *dx, \
t_parser_info *p)
{
	ft_strcpy(&dx->expanded[dx->new_index], ft_getenv(dx->i, str, p));
	dx->new_index += ft_strlen(ft_getenv(dx->i, str, p));
	skip_dollar_content(&dx->i, str);
}

void	expand_dollar_quote(int type, char *str, t_dollar_expansion *dx, \
t_parser_info *p)
{
	dx->expanded[dx->new_index++] = str[dx->i++];
	while (ft_isquote(str[dx->i]) != type && str[dx->i])
	{
		if (str[dx->i] == '$' && str[dx->i + 1] == '?')
			expand_dollar_question(dx, p);
		else if (str[dx->i] == '$' && ft_isdigit(str[dx->i + 1]) == 1)
			dx->i += 2;
		else if (str[dx->i] == '$' && ft_isspace(str[dx->i + 1]) == 0 && \
		ft_ismeta(&str[dx->i + 1], dx->meta) == 0 && str[dx->i + 1] && \
		ft_isquote(str[dx->i + 1]) == 0)
			expand_dollar_general(str, dx, p);
		else
			dx->expanded[dx->new_index++] = str[dx->i++];
	}
	if (str[dx->i])
		dx->expanded[dx->new_index++] = str[dx->i++];
}

void	expand_dollar_single_quote(bool append_flag, char *str, \
t_dollar_expansion *dx, t_parser_info *p)
{
	if (append_flag == true)
		expand_dollar_quote(1, str, dx, p);
	else
	{
		dx->expanded[dx->new_index++] = str[dx->i++];
		while (ft_isquote(str[dx->i]) != 1 && str[dx->i])
			dx->expanded[dx->new_index++] = str[dx->i++];
		if (str[dx->i])
			dx->expanded[dx->new_index++] = str[dx->i++];
	}
}
