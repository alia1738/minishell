/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 15:22:39 by anasr             #+#    #+#             */
/*   Updated: 2022/04/04 14:19:52 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_meta(char	**meta)
{
	meta[0] = "<<";
	meta[1] = "<";
	meta[2] = ">>";
	meta[3] = ">";
	meta[4] = "|";
	meta[5] = "=";
	meta[6] = "+";
	meta[7] = ".";
	meta[8] = ",";
	meta[9] = ":";
	meta[10] = "^";
	meta[11] = "~";
	meta[12] = "-";
	meta[13] = "/";
	meta[14] = 0;
}

char	*ft_getenv(int i, char *str, t_parser_info *p)
{
	int		j;
	char	*meta[15];
	char	temp[1024];

	init_meta(meta);
	j = 0;
	ft_bzero(temp, 1024);
	while (str[++i] != '$' && ft_ismeta(&str[i], meta) == 0 && \
	ft_isquote(str[i]) == 0 && ft_isspace(str[i]) == 0 && str[i])
	{
		temp[j++] = str[i];
	}
	return (local_getenv(temp, p->env));
}

void	skip_dollar_content(int *i, char *str)
{
	char	*meta[15];

	(*i)++;
	init_meta(meta);
	while (str[*i] != '$' && ft_ismeta(&str[*i], meta) == 0 && \
	ft_isquote(str[*i]) == 0 && ft_isspace(str[*i]) == 0 && str[*i])
		(*i)++;
}

int	len_with_expansion(char	*str, t_parser_info *p, bool append_flag)
{
	t_dollar_expansion	dxl;

	ft_bzero(&dxl, sizeof(dxl));
	init_meta(dxl.meta);
	dxl.i = 0;
	dxl.len = 0;
	while (str[dxl.i])
	{
		if (str[dxl.i] == '$' && str[dxl.i + 1] == '?')
			len_dollar_question(&dxl, p);
		else if (str[dxl.i] == '$' && ft_isdigit(str[dxl.i + 1]) == 1)
			dxl.i += 2;
		else if (str[dxl.i] == '$' && ft_isspace(str[dxl.i + 1]) == 0 && \
		ft_ismeta(&str[dxl.i + 1], dxl.meta) == 0 && str[dxl.i + 1])
			len_dollar_general(&dxl, str, p);
		else if (ft_isquote(str[dxl.i]) == 1)
			len_dollar_single_quote(append_flag, str, &dxl, p);
		else if (ft_isquote(str[dxl.i]) == 2)
			len_dollar_quote(2, str, &dxl, p);
		else
			increment_len_n_index(&dxl);
	}
	return (dxl.len);
}

/*This function occurs before trimming the quotes, so it's removed now*/

char	*expand_dollars_in_str(char *str, t_parser_info *p, bool append_flag)
{
	t_dollar_expansion	dx;

	ft_bzero(&dx, sizeof(dx));
	init_meta(dx.meta);
	dx.expanded = ft_calloc_p(len_with_expansion(str, p, append_flag) + 1, \
	sizeof(char));
	dx.i = 0;
	dx.new_index = 0;
	while (str[dx.i])
	{
		if (str[dx.i] == '$' && str[dx.i + 1] == '?')
			expand_dollar_question(&dx, p);
		else if (str[dx.i] == '$' && ft_isdigit(str[dx.i + 1]) == 1)
			dx.i += 2;
		else if (str[dx.i] == '$' && ft_isspace(str[dx.i + 1]) == 0 && \
		ft_ismeta(&str[dx.i + 1], dx.meta) == 0 && str[dx.i + 1])
			expand_dollar_general(str, &dx, p);
		else if (ft_isquote(str[dx.i]) == 1)
			expand_dollar_single_quote(append_flag, str, &dx, p);
		else if (ft_isquote(str[dx.i]) == 2)
			expand_dollar_quote(2, str, &dx, p);
		else
			dx.expanded[dx.new_index++] = str[dx.i++];
	}
	return (dx.expanded);
}
