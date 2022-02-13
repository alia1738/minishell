/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 13:23:51 by anasr             #+#    #+#             */
/*   Updated: 2022/02/12 12:49:29 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	c = 0;
	if (n < 0)
	{
		if (n == INT_MIN)
			write(fd, "-2147483648", 11);
		else
		{
			write(fd, "-", sizeof(char));
			n *= -1;
		}
	}
	if (n >= 0 && n <= 9)
	{
		c = n + 48;
		write(fd, &c, sizeof(char));
	}
	if (n > 9)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
}
