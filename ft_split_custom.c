/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_custom.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 18:10:48 by anasr             #+#    #+#             */
/*   Updated: 2022/04/04 12:20:24 by anasr            ###   ########.fr       */
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

int		ft_isquote(char c)
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

char *strcpy_wout_quotes(char *str)
{
	int		i;
	int		new;
	int		quote_type;
	int		len_wout_quote;
	char	*new_str;

	i = 0;
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
			i++;
		}
	}
	
	new_str = ft_calloc_p(len_wout_quote + 1, sizeof(char));
	if (!new_str)
		return (NULL);
	i = 0;
	new = 0;
	while (str[i])
	{
		while (ft_isquote(str[i]) == 0 && str[i])
		{
			new_str[new++] = str[i];
			i++;
		}
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

static int	count_inputs(char *input, char **meta)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (input[i])
	{
		skip_isspaces(&i, input);
		if (input[i] == '\0')
			return (count);
		if (ft_ismeta(&input[i], meta) > 0)
		{
			count++;
			i += ft_ismeta(&input[i], meta);
		}
		else if (ft_isspace(input[i]) == 0)
		{
			count++;
			while ((ft_isspace(input[i]) == 0 && ft_ismeta(&input[i], meta) == 0 && input[i]))
			{
				if (ft_isquote (input[i]) > 0)
					skip_quote_content(&i, input);
				else
					i++;
			}
		}
		else
			i++;
	}
	return (count);
}

static void	go_to_next_word(int *i, char *input, char **meta)
{
	if (ft_ismeta(&input[*i], meta) > 0)
	{
		(*i) += ft_ismeta(&input[*i], meta);
		return ;
	}
	while (ft_isspace(input[*i]) == 0 && ft_ismeta(&input[*i], meta) == 0 && input[*i])
	{
		if (ft_isquote(input[*i]) > 0)
			skip_quote_content(i, input);
		else
			(*i)++;
	}
	return ;
}

static int	get_next_word_len(int i, char *input, char **meta)
{
	int	len;

	len = 0;
	if (ft_ismeta(&input[i], meta) > 0)
		return (ft_ismeta(&input[i], meta));
	while (ft_isspace(input[i]) == 0 && ft_ismeta(&input[i], meta) == 0 && input[i])
	{
		if (ft_isquote (input[i]) > 0)
			len += skip_quote_content(&i, input);
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

static char	*get_next_word(int i, char *input, char **meta, t_parser_info *p)
{
	int		len;
	char	*temp;

	(void)p;
	len = get_next_word_len(i, input, meta);
	temp = ft_substr(input, i, len);
	return (temp);
}

char	**ft_split_custom(char *input, char **meta, t_parser_info *p)
{
	int		i;
	char	**result;
	int		word_count;
	int		word_index;

	if (ft_ismeta("|", meta) > 0)
		return (ft_split(input, '|'));
	i = 0;
	word_index = 0;
	word_count = count_inputs(input, meta);
	result = ft_calloc_p(word_count + 1, sizeof(char *)); //protect
	skip_isspaces(&i, input);
	while (word_index < word_count)
	{
		result[word_index] = get_next_word(i, input, meta, p);
		word_index++;
		go_to_next_word(&i, input, meta);
		skip_isspaces(&i, input);
	}
	return (result);
}
