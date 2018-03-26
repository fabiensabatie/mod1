/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vecbew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 16:49:41 by fsabatie          #+#    #+#             */
/*   Updated: 2018/02/04 13:18:51 by fsabatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_vec	*ft_vecnew(size_t size, int datasize)
{
	t_vec *vec;

	if (!(vec = (t_vec*)malloc(sizeof(t_vec))))
		return (NULL);
	if (!(vec->buffer = (void*)malloc(size * datasize)))
		return (NULL);
	vec->size = size;
	vec->data_size = datasize;
	vec->occ = 0;
	vec->row = 1;
	return (vec);
}
