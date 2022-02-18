/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 13:28:35 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/02/16 16:41:34 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_c(char const *s, char c)
{
	int	i;
	int	ii;

	i = 0;
	ii = 0;
	while (s[i])
	{
		if (s[i] == c)
			ii++;
		i++;
		ii++;
	}
	if (i == ii)
		return (1);
	return (0);
}

static t_fullcount	count(const char *s, char c)
{
	t_fullcount	r;
	int			f_count;
	int			s_count;

	f_count = 0;
	s_count = 0;
	while (s[f_count])
	{
		if (s[f_count] != c)
		{
			if (s[f_count] && s[f_count + 1] == c)
				s_count++;
		}
		f_count++;
	}
	if (f_count > 0 && s[f_count - 1] != c)
		s_count++;
	if (check_c(s, c) && f_count > 0)
		s_count = 1;
	r.full = f_count;
	r.str_c = s_count;
	return (r);
}

static t_count_s	s_len(const char *s, char c)
{
	static int	ii_count[2] = {0, 0};
	t_variables	hi;

	hi.fcount = count(s, c);
	hi.str_l = 0;
	while (ii_count[0] < hi.fcount.full)
	{
		if (s[ii_count[0]] != c && ++(hi.str_l))
		{
			if (s[ii_count[0] + 1] == c || s[ii_count[0] + 1] == 0)
			{
				hi.index.i = ii_count[0]++;
				hi.index.str_l = hi.str_l;
				if (hi.fcount.str_c == ++(ii_count[1]))
					ft_bzero(ii_count, (2 * sizeof(int)));
				return (hi.index);
			}
		}
		ii_count[0]++;
	}
	hi.index.i = ii_count[0];
	hi.index.str_l = 0;
	if (s[ii_count[0]] == 0)
		ft_bzero(ii_count, (2 * sizeof(int)));
	return (hi.index);
}

static char	*create_str(const char *s, char c)
{
	int			i;
	char		*str;
	t_count_s	len;

	if (check_c(s, c))
	{
		str = ft_strdup(s);
		return (str);
	}
	len = s_len(s, c);
	i = len.str_l;
	str = (char *)malloc((i + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[i--] = 0;
	while (i >= 0)
		str[i--] = s[len.i--];
	return (str);
}

char	**ft_split(char const *s, char c)
{
	int			i;
	t_fullcount	n;
	char		**array;

	n = count(s, c);
	array = (char **)malloc((n.str_c + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	if (!s[0])
	{
		array[0] = 0;
		return (array);
	}
	i = 0;
	while (i < n.str_c)
	{
		array[i] = create_str(s, c);
		if (array[i++] == NULL)
			return (NULL);
	}
	array[i] = 0;
	return (array);
}
