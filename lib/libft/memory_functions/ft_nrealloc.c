/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nrealloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/08 21:29:40 by fsabatie          #+#    #+#             */
/*   Updated: 2018/03/08 18:59:47 by fsabatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_nrealloc(void *old, size_t size, size_t elems)
{
	void *ret;

	if (!(ret = (ft_memalloc(size * (elems + 1)))))
		return (NULL);
	ft_memcpy(ret, old, elems * size);
	return (ret);
}
