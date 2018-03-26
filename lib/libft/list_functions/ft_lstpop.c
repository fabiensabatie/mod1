/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/10 12:51:35 by fsabatie          #+#    #+#             */
/*   Updated: 2018/02/16 14:06:44 by fsabatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Renvoie le nth element de la liste.
*/

t_list	*ft_lstpop(t_list **alst, size_t n)
{
	t_list	*start;
	size_t	i;

	i = 0;
	start = *alst;
	while (start->next && i < n)
		start = start->next;
	return (start);
}
