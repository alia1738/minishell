/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 12:14:38 by anasr             #+#    #+#             */
/*   Updated: 2022/02/13 14:09:20 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*ptr;

	i = 0;
	if (!s)
		return (NULL);
	if (len < ft_strlen(s))
		ptr = malloc(len + 1);
	else
		ptr = malloc(ft_strlen(s) + 1);
	if (ptr == NULL)
		return (NULL);
	if (start <= ft_strlen(s) + 1)
	{
		while (i < len && s[i])
		{
			ptr[i] = s[start + i];
			i++;
		}
	}
	ptr[i] = '\0';
	return (ptr);
}
