/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 00:54:29 by anasr             #+#    #+#             */
/*   Updated: 2022/02/14 04:37:12 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	size_t		i;
	int			sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (ft_isdigit(str[i]) == 1)
	{
		if (result >= LONG_MAX / 10 && str[i] > '7')
			return (-1);
		if (result >= LONG_MAX / 10 && str[i] > '6' && sign == -1)
			return (0);
		result = 10 * result + (str[i] - 48);
		i++;
	}
	return (result * sign);
}
