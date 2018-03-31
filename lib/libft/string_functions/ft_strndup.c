/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlay <vlay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/10 16:07:17 by vlay              #+#    #+#             */
/*   Updated: 2018/03/31 19:17:28 by vlay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char *str;

	if (!(str = ft_strnew(n)))
		return (NULL);
	ft_strncpy(str, (char *)s, n);
	str[n] = '\0';
	return (str);
}
