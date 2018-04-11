/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/10 12:51:35 by fsabatie          #+#    #+#             */
/*   Updated: 2018/02/16 13:57:32 by fsabatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
** Ajoute l’élément new en tête de la liste.
*/

void	ft_lstadd(t_list **alst, t_list *new)
{
	if (*alst && new) {
		new->next = *alst;
	}
	*alst = new;
}
