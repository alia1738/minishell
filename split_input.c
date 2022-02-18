/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 13:51:32 by anasr             #+#    #+#             */
/*   Updated: 2022/02/18 19:14:02 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_inputs(char *input)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	skip_isspaces(&i, input);
	while (input[i])
	{
		skip_isspaces(&i, input);
		if (input[i] == '\0')
			return (count);
		if (ft_strncmp("<<", &input[i], 2) == 0 || ft_strncmp(">>", &input[i], 2) == 0)
		{
			count++;
			i++;
		}
		else if (input[i] == '<' || input[i] == '>')
			count++;
		else if (ft_isspace(input[i]) == 0)
		{
			count++;
			while (ft_isspace(input[i]) == 0 && input[i] != '<' && input[i] != '>' && input[i])
				i++;
			if (input[i] == '\0' || input[i] == '<' || input[i] == '>')
				continue ;
		}
		i++;
	}
	return (count);
}

int	get_next_word_len(int i, char *input)
{
	int	len;

	len = 0;
	if (ft_strncmp("<<", &input[i], 2) == 0 || ft_strncmp(">>", &input[i], 2) == 0 )
		return (2);
	if (input[i] == '<' || input[i] == '>')
		return (1);
	while (ft_isspace(input[i]) == 0 && input[i] != '<' && input[i] != '>' && input[i])
	{
		len++;
		i++;
	}
	return (len);
}

char	*get_next_word(int i, char *input)
{
	int	len;

	len = get_next_word_len(i, input);
	return (ft_substr(input, i, len));
}
	// printf("len = %d word = #%s#\n", len, ft_substr(input, i, len));

void	go_to_next_word(int *i, char *input)
{
	if (ft_strncmp("<<", &input[*i], 2) == 0 || ft_strncmp(">>", &input[*i], 2) == 0)
	{
		(*i) += 2;
		skip_isspaces(i, input);
		return ;
	}
	if (input[*i] == '<' || input[*i] == '>')
	{
		(*i)++;
		skip_isspaces(i, input);
		return ;
	}
	while (ft_isspace(input[*i]) == 0 && input[*i] != '<' && input[*i] != '>' && input[*i])
		(*i)++;
	skip_isspaces(i, input);
	return ;
}
	// printf("NEXT WORD: ##%s##\n", &input[*i]);

void	split_input(char *input, t_parser_info *p)
{
	int	i;
	int	word_index;
	int	word_count;

	i = 0;
	word_index = 0;
	word_count = count_inputs(input);
	p->words = ft_calloc(word_count + 1, sizeof(char *));
	skip_isspaces(&i, input);
	while (word_index < word_count)
	{
		p->words[word_index++] = get_next_word(i, input);
		go_to_next_word(&i, input);
	}
}
