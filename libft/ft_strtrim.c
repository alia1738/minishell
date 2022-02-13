/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 12:12:37 by anasr             #+#    #+#             */
/*   Updated: 2022/02/12 13:11:19 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_index_plus(char const *s1, char const *set, size_t i)
{
	size_t	j;

	j = 0;
	while (s1[i])
	{
		j = 0;
		while (set[j])
		{
			if (s1[i] == set[j])
				break ;
			j++;
		}
		if (set[j] == '\0')
			break ;
		i++;
	}
	return (i);
}

static size_t	ft_index_minus(char const *s1, char const *set, size_t i)
{
	size_t	j;

	j = 0;
	while (i > 0)
	{
		j = 0;
		while (set[j])
		{
			if (s1[i - 1] == set[j])
				break ;
			j++;
		}
		if (set[j] == '\0')
			break ;
		i--;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	x;

	i = 0;
	i = ft_index_plus(s1, set, i);
	x = ft_strlen(s1);
	if (i == x)
		return (ft_strdup(""));
	x = ft_index_minus(s1, set, x);
	return (ft_substr(s1, i, x - i));
}
