/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 14:49:00 by anasr             #+#    #+#             */
/*   Updated: 2021/10/11 21:32:20 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*temp;

	current = *lst;
	temp = current;
	while (current->next != NULL)
	{
		del(current->content);
		temp = current;
		current = current->next;
		free(temp);
	}
	del(current->content);
	free(current);
	*lst = NULL;
}
