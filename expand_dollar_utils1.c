/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_utils1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 11:49:56 by anasr             #+#    #+#             */
/*   Updated: 2022/04/04 14:21:01 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	len_dollar_question(t_dollar_expansion *dxl, t_parser_info *p)
{
	char	*temp;

	temp = ft_itoa(p->exit_code);
	dxl->len += ft_strlen(temp);
	dxl->i += 2;
	free(temp);
}

void	len_dollar_general(t_dollar_expansion *dxl, char *str, t_parser_info *p)
{
	dxl->len += ft_strlen(ft_getenv(dxl->i, str, p));
	skip_dollar_content(&dxl->i, str);
}

void	increment_len_n_index(t_dollar_expansion *dxl)
{
	dxl->len++;
	dxl->i++;
}

void	len_dollar_quote(int type, char *str, t_dollar_expansion *dxl, \
t_parser_info *p)
{
	increment_len_n_index(dxl);
	while (ft_isquote(str[dxl->i]) != type && str[dxl->i])
	{
		if (str[dxl->i] == '$' && str[dxl->i + 1] == '?')
			len_dollar_question(dxl, p);
		else if (str[dxl->i] == '$' && ft_isdigit(str[dxl->i + 1]) == 1)
			dxl->i += 2;
		else if (str[dxl->i] == '$' && ft_isspace(str[dxl->i + 1]) == 0 && \
		ft_ismeta(&str[dxl->i + 1], dxl->meta) == 0 && str[dxl->i + 1])
			len_dollar_general(dxl, str, p);
		else
			increment_len_n_index(dxl);
	}
	if (str[dxl->i])
		increment_len_n_index(dxl);
}

void	len_dollar_single_quote(bool append_flag, char *str, \
t_dollar_expansion *dxl, t_parser_info *p)
{
	if (append_flag == true)
		len_dollar_quote(1, str, dxl, p);
	else
		dxl->len += skip_quote_content(&dxl->i, str);
}
