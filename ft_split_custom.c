/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_custom.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 18:10:48 by anasr             #+#    #+#             */
/*   Updated: 2022/02/24 12:56:08 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_ismeta(char *current_c, char **meta)
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
		else if (input[i] == '\"')// account for unclosed quotes
		{
			count++;
			while (input[++i] != '\"')
				;
			++i;
		}
		else if (input[i] == '\'')// account for unclosed quotes
		{
			count++;
			while (input[++i] != '\'')
				;
			++i;
		}
		else if (ft_isspace(input[i]) == 0)
		{
			count++;
			while (ft_isspace(input[i]) == 0 && ft_ismeta(&input[i], meta) == 0 && input[i])
				i++;
			if (input[i] == '\0' || ft_ismeta(&input[i], meta) > 0)
				continue ;
		}
		else
			i++;
	}
	return (count);
}

static void	go_to_next_word(int *i, char *input, char **meta)
{
	if (input[*i] == '\"' || input[*i] == '\'')
	{
		if (input[*i] == '\"')
		{
			while (input[++(*i)] != '\"')
				;
		}
		else
		{
			while (input[++(*i)] != '\'')
				;
		}
		++(*i);
		return ;
	}
	else if (ft_ismeta(&input[*i], meta) > 0)
	{
		(*i) += ft_ismeta(&input[*i], meta);
		return ;
	}
	while (ft_isspace(input[*i]) == 0 && ft_ismeta(&input[*i], meta) == 0 && input[*i])
		(*i)++;
	return ;
}

static int	get_next_word_len(int i, char *input, char **meta)
{
	int	len;

	len = 0;
	if (input[i] == '\"' || input[i] == '\'')
	{
		if (input[i] == '\"')
		{
			while (input[++i] != '\"')
				len++;
		}
		else
		{
			while (input[++i] != '\'')
				len++;
		}
		return (len + 2);
	}
	else if (ft_ismeta(&input[i], meta) > 0)
		return (ft_ismeta(&input[i], meta));
	while (ft_isspace(input[i]) == 0 && ft_ismeta(&input[i], meta) == 0 && input[i])
	{
		len++;
		i++;
	}
	return (len);
}

static char	*get_next_word(int i, char *input, char **meta, t_parser_info *p)
{
	int		len;
	char	*temp;
	char	*result;
	

	len = get_next_word_len(i, input, meta);
	temp = ft_substr(input, i, len);
	p->do_not_expand[p->word_index] = false;
	if (*temp == '\"')
		result = ft_strtrim(temp, "\"");
	else if (*temp == '\'')
	{
		result = ft_strtrim(temp, "'");
		p->do_not_expand[p->word_index] = true;
	}
	else
		return (temp);
	free(temp);
	return (result);
}
	// printf("WOO--len: %d .. word: %s\n", len, temp);
	// printf("len: %d .. word: %s\n", len, result);

char	**ft_split_custom(char *input, char **meta, t_parser_info *p)
{
	int	i;
	int	word_count;
	char	**result;

	if (ft_ismeta("|", meta) > 0)
		return (ft_split(input, '|'));
	i = 0;
	p->word_index = 0;
	word_count = count_inputs(input, meta);
	result = ft_calloc(word_count + 1, sizeof(char *)); //protect
	skip_isspaces(&i, input);
	while (p->word_index < word_count)
	{
		result[p->word_index] = get_next_word(i, input, meta, p);
		p->word_index++;
		go_to_next_word(&i, input, meta);
		skip_isspaces(&i, input);
	}
	return (result);
}

// int main (int argc, char **argv)
// {
// 	char	**ptr;
// 	char	*meta[5] = {"<<", "<", ">", ">>", 0};
	
// 	if (argc < 2)
// 		exit(EXIT_FAILURE);
// 	ptr = ft_split_custom(argv[1], meta);
// 	while (*ptr)
// 	{
// 		printf("%s\n", *ptr);
// 		ptr++;
// 	}
// 	// printf("count: %d\n", count_inputs(argv[1], meta));
// }

	// *meta = "<<";
	// *(meta + 1) = "<";
	// *(meta + 2) = ">";
	// *(meta + 3) = ">>";
	// *(meta + 4) = 0;
	