/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 11:49:56 by anasr             #+#    #+#             */
/*   Updated: 2022/03/29 12:30:07 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	len_dollar_question(int *i, int *len, t_parser_info *p)
{
	char	*temp;
	
	temp = ft_itoa(p->exit_code);
	(*len) += ft_strlen(temp);
	(*i) += 2;
	free(temp);
}

void	expand_dollar_question(int *i, int *new_index, char *expanded, t_parser_info *p)
{
	char	*temp;

	temp = ft_itoa(p->exit_code);
	ft_strcpy(&expanded[*new_index], temp);
	(*new_index) += ft_strlen(temp);
	free(temp);
	(*i) += 2;
}

void	len_dollar_general(int *i, int *len, char *str, t_parser_info *p)
{
	(*len) += ft_strlen(ft_getenv((*i), str, p));
	skip_dollar_content(i, str);
}

void	expand_dollar_general(int *i, int *new_index, char *str, char *expanded, t_parser_info *p)
{
	ft_strcpy(&expanded[*new_index], ft_getenv((*i), str, p));
	(*new_index) = ft_strlen(ft_getenv((*i), str, p));
	skip_dollar_content(i, str);
}