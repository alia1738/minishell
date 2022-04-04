/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_helpers2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 16:43:16 by aalsuwai          #+#    #+#             */
/*   Updated: 2022/04/04 11:50:39 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_str_isdigit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

char	*ft_str_tolower(char *str)
{
	int		i;

	i = -1;
	while (str[++i])
		str[i] = ft_tolower(str[i]);
	return (str);
}

int	compare_caseless(const char *s_unknown, const char *s_lowercase)
{
	int		i;
	char	c;

	if (ft_strlen(s_unknown) != ft_strlen(s_lowercase))
		return (0);
	i = -1;
	while (s_lowercase[++i])
	{
		c = s_unknown[i];
		if (ft_tolower(c) != s_lowercase[i])
			return (0);
	}
	return (1);
}

int		check_longmax(char *str)
{
	int			i;
	int			sign;
	int			count;
	long long	result;

	i = 0;
	sign = 1;
	count = 0;
	result = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]) == 1)
	{
		if (result >= 9223372036854775807 / 10 && str[i] > '7' && sign == 1)
			return (1);
		if ((result >= 9223372036854775807 / 10 && str[i] > '8') || count > 19)
			return (1);
		result = 10 * result + (str[i] - 48);
		i++;
		count++;
	}
	return (0);
}
