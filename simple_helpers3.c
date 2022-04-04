/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_helpers3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 11:50:21 by anasr             #+#    #+#             */
/*   Updated: 2022/04/04 12:15:28 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_smartncmp(const char *s1, const char *s2, size_t len)
{
	if (ft_strlen(s1) == ft_strlen(s2) && !ft_strncmp(s1, s2, len))
		return (1);
	return (0);
}

void	*ft_calloc_p(size_t count, size_t size)
{
	void	*ptr;

	ptr = ft_calloc(count, size);
	if (!ptr)
	{
		printf("Memory Issue: memory allocation failed\n");
		exit(1);
	}
	return (ptr);
}