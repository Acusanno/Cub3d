/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 12:50:58 by acusanno          #+#    #+#             */
/*   Updated: 2020/11/27 15:16:05 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_list	*ft_lstmap(t_list *lst, void *(*f) (void *), void (*del) (void *))
{
	t_list	*ptr;
	t_list	*temp;

	if (lst == NULL)
		return (NULL);
	ptr = NULL;
	while (lst != NULL)
	{
		if (!(temp = ft_lstnew(f(lst->content))))
		{
			ft_lstclear(&ptr, del);
			return (NULL);
		}
		ft_lstadd_back(&ptr, temp);
		lst = lst->next;
	}
	return (ptr);
}
