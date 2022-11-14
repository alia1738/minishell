/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 13:22:51 by anasr             #+#    #+#             */
/*   Updated: 2022/04/04 18:07:54 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_empty_input(char	*input)
{
	int	i;

	i = 0;
	skip_isspaces(&i, input);
	if (input[i])
		return (1);
	free(input);
	return (0);
}

static int	set_err(t_parser_info *p)
{
	p->exit_code = 258;
	printf("babyshell: syntax error regarding the usage of metacharacters\n");
	free(p->input);
	return (1);
}

static void	increment_n_reset(int flag, int *i, char **meta, t_parser_info *p)
{
	if (!flag)
	{
		*i = 0;
		meta[0] = "<<";
		meta[1] = "<";
		meta[2] = ">>";
		meta[3] = ">";
		meta[4] = "|";
		meta[5] = 0;
	}
	else
		(*i)++;
	p->meta_place_taken = false;
	p->meta_pipe = false;
}

static void	if_meta_helper(int *i, char *input, char **meta, t_parser_info *p)
{
	if (!ft_strncmp(&input[*i], "|", 1))
		p->meta_pipe = true;
	else
		p->meta_place_taken = true;
	*i += ft_ismeta(&input[*i], meta);
}

int	check_repeated_meta(char *input, t_parser_info *p)
{
	int		i;
	char	*meta[6];

	increment_n_reset(0, &i, meta, p);
	skip_isspaces(&i, input);
	if (!ft_strncmp("|", &input[i], 1) && set_err(p) == 1)
		return (-1);
	while (input[i])
	{
		if (((ft_ismeta(&input[i], meta) > 0 && p->meta_place_taken) || \
		(!ft_strncmp(&input[i], "|", 1) && p->meta_pipe)) && set_err(p) == 1)
			return (-1);
		else if (ft_ismeta(&input[i], meta) > 0)
			if_meta_helper(&i, input, meta, p);
		else if (ft_isquote(input[i]) > 0)
			skip_quote_content(&i, input);
		else if (ft_isspace(input[i]) == 1)
			skip_isspaces(&i, input);
		else
			increment_n_reset(1, &i, meta, p);
	}
	if ((p->meta_place_taken == true || p->meta_pipe == true) && \
		set_err(p) == 1)
		return (-1);
	return (0);
}
