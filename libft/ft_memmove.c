/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 20:56:45 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/02 22:52:28 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*temp_dst;
	unsigned char	*temp_src;
	size_t			cont;

	cont = -1;
	temp_dst = (unsigned char *) dst;
	temp_src = (unsigned char *) src;
	if (!temp_dst && !temp_src)
		return (NULL);
	if (temp_dst <= temp_src)
	{
		while (++cont < len)
			temp_dst[cont] = temp_src[cont];
	}
	else
	{
		cont = len;
		while (cont > 0)
		{
			cont--;
			temp_dst[cont] = temp_src[cont];
		}
	}
	return (dst);
}
