/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 14:29:53 by anasr             #+#    #+#             */
/*   Updated: 2022/02/14 04:36:38 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (dst == NULL && src == NULL)
		return (0);
	if (dst < src)
		return (ft_memcpy(dst, src, n));
	else
	{
		i = n - 1;
		while (n--)
		{
			((char *)dst)[i] = ((char *)src)[i];
			i--;
		}
		return (dst);
	}
}
