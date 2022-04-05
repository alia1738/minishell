/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 14:39:21 by anasr             #+#    #+#             */
/*   Updated: 2022/04/04 15:00:27 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_ismeta(char *current_c, char **meta)
{
	int	i;

	i = -1;
	while (meta[++i])
	{
		if (!ft_strncmp(current_c, meta[i], ft_strlen(meta[i])))
			return (ft_strlen(meta[i]));
	}
	return (0);
}

int	ft_isquote(char c)
{
	if (c == '\'')
		return (1);
	else if (c == '\"')
		return (2);
	return (0);
}

int	skip_quote_content(int *i, char *input)
{
	int	quote_len;
	int	quote_type;

	quote_len = 1;
	quote_type = ft_isquote(input[*i]);
	(*i)++;
	while (ft_isquote(input[*i]) != quote_type && input[*i])
	{
		(*i)++;
		quote_len++;
	}
	if (ft_isquote(input[*i]) == quote_type)
	{
		(*i)++;
		quote_len++;
	}
	return (quote_len);
}

static int	len_wout_quotes(char *str)
{
	int	i;
	int	quote_type;
	int	len_wout_quote;

	i = 0;
	quote_type = 0;
	len_wout_quote = 0;
	while (str[i])
	{
		while (ft_isquote(str[i]) == 0 && str[i])
		{
			i++;
			len_wout_quote++;
		}
		if (str[i])
		{
			quote_type = ft_isquote(str[i]);
			while (ft_isquote(str[++i]) != quote_type && str[i])
				len_wout_quote++;
			if (str[i])
				i++;
		}
	}
	return (len_wout_quote);
}

char	*strcpy_wout_quotes(char *str)
{
	int		i;
	int		new;
	int		quote_type;
	char	*new_str;

	new_str = ft_calloc_p(len_wout_quotes(str) + 1, sizeof(char));
	i = 0;
	new = 0;
	while (str[i])
	{
		while (ft_isquote(str[i]) == 0 && str[i])
			new_str[new++] = str[i++];
		if (str[i])
		{
			quote_type = ft_isquote(str[i]);
			while (ft_isquote(str[++i]) != quote_type && str[i])
				new_str[new++] = str[i];
			if (str[i])
				i++;
		}
	}
	free(str);
	return (new_str);
}
