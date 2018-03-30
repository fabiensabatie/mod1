/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vecinit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 16:49:41 by fsabatie          #+#    #+#             */
/*   Updated: 2018/02/04 13:18:51 by fsabatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#define VS vec->size
#define VB vec->buffer
#define VDS vec->data_size
#define VO vec->occ
#define VR vec->row

t_vec	*ft_vecinit(t_vec *vec)
{
	ft_bzero(vec->buffer, VS * VDS)
	VO = 0;

	return (vec);
}
