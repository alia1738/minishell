/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 17:52:09 by anasr             #+#    #+#             */
/*   Updated: 2022/02/24 12:45:26 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **array)
{
	int	i;

	i = -1;
	while (array[++i])
		free(array[i]);
	return ;
}

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

void	clear(void)
{
	printf("\e[1;1H\e[2J"); //this one doesn't delete history
	// printf("\033c"); //this one deletes the history of the terminal
}

char	*ft_strcpy(char *dst, const char *src)
{
	int i;
	i = 0;
	while (src && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
