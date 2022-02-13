/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasr <anasr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 15:05:04 by anasr             #+#    #+#             */
/*   Updated: 2021/10/12 14:32:40 by anasr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*head;
	t_list	*check;

	check = NULL;
	if (lst == NULL)
		return (NULL);
	head = NULL;
	while (lst != NULL)
	{
		check = ft_lstnew(f(lst->content));
		if (check == NULL)
		{
			ft_lstclear(&head, del);
			return (NULL);
		}
		ft_lstadd_back(&head, check);
		lst = lst->next;
	}
	return (head);
}
