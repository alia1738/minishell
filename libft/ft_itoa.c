/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 12:40:34 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/02/14 05:13:04 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count(int n)
{
	int	c;
	int	count;

	c = n;
	count = 0;
	while (c != 0)
	{
		c = c / 10;
		count++;
	}
	if (n == 0)
		count++;
	if (n < 0)
		return (count + 1);
	return (count);
}

static char	*copy_part(int n, int save, char *s, int c)
{
	while (c > 0)
	{
		s[c--] = (n % 10) + 48;
		n = n / 10;
	}
	if (save > 0)
		s[0] = n + 48;
	else if (save < 0)
		s[0] = '-';
	else if (save == 0)
		s[0] = '0';
	return (s);
}

char	*ft_itoa(int n)
{
	char	*s;
	int		c;
	int		save;

	save = n;
	c = count(n);
	if (n < 0 && n != INT_MIN)
		n = n * -1;
	s = (char *)malloc((c + 1) * sizeof(char));
	if (s == 0)
		return (NULL);
	s[c--] = 0;
	if (save == INT_MIN)
	{
		s[c--] = '8';
		n = 214748364;
	}
	s = copy_part(n, save, s, c);
	return (s);
}
