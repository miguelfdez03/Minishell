/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:21:54 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/02 23:11:52 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	src_len;
	size_t	cont;

	cont = 0;
	src_len = ft_strlen(src);
	if (dstsize == 0)
		return (src_len);
	while (cont < dstsize - 1 && src[cont])
	{
		dst[cont] = src[cont];
		cont++;
	}
	dst[cont] = '\0';
	return (src_len);
}
