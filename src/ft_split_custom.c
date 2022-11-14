/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_custom.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 18:10:48 by anasr             #+#    #+#             */
/*   Updated: 2022/04/04 15:22:46 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			count_inputs_helper(&i, input, meta);
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
	while (ft_isspace(input[*i]) == 0 && ft_ismeta(&input[*i], meta) == 0 \
	&& input[*i])
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
	while (ft_isspace(input[i]) == 0 && ft_ismeta(&input[i], meta) == 0 \
	&& input[i])
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
	result = ft_calloc_p(word_count + 1, sizeof(char *));
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
