/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_helpers1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 17:52:09 by anasr             #+#    #+#             */
/*   Updated: 2022/04/04 16:14:34 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(const char *s1, int n)
{
	int		i;
	char	*dup;	

	i = 0;
	if (!s1)
		return (NULL);
	dup = (char *)malloc(n + 1);
	if (!dup)
		return (NULL);
	while (s1 && i < n)
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	skip_isspaces(int *index, char *input)
{
	while (ft_isspace(input[*index]) == 1)
		(*index)++;
}

int	clear(t_parser_info *p)
{
	printf("\e[1;1H\e[2J");
	p->exit_code = 0;
	return (0);
}

char	*ft_strcpy(char *dst, const char *src)
{
	int	i;

	i = 0;
	while (src && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	**dup_array(char **a1)
{
	int		i;
	char	**new_a;

	i = 0;
	while (a1[i])
		i++;
	new_a = ft_calloc_p((i + 1), sizeof(char *));
	i = 0;
	while (a1[i])
	{
		new_a[i] = ft_strdup(a1[i]);
		i++;
	}
	new_a[i] = 0;
	return (new_a);
}
