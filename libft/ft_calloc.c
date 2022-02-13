/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Alia <Alia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 19:41:02 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/02/12 15:56:13 by Alia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	i;
	void	*s;

	i = 0;
	s = (void *)malloc(count * size);
	if (s == 0)
		return (NULL);
	while (i < count * size)
		((char *)s)[i++] = 0;
	return (s);
}
