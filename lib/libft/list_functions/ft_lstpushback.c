/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpushback.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/10 12:51:35 by fsabatie          #+#    #+#             */
/*   Updated: 2018/02/16 14:02:31 by fsabatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Ajoute l’élément new en fin de liste.
*/

void	ft_lstpushback(t_list *start, t_list *new)
{
	while (start->next)
		start = start->next;
	start->next = new;
}
