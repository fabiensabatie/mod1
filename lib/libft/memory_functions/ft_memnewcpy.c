/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 21:29:08 by fsabatie          #+#    #+#             */
/*   Updated: 2018/01/25 14:51:24 by fsabatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memnewcpy(void *src, size_t n, size_t len)
{
	unsigned char *dest;

	if (!(dest = (unsigned char*)malloc(n * len)))
		return (NULL);
	ft_memcpy(dest, src, n * len);
	return ((void*)dest);
}
