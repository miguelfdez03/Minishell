/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:19:15 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/09 16:24:46 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*temp_dest;
	unsigned char	*temp_src;
	size_t			cont;

	cont = 0;
	temp_dest = (unsigned char *) dest;
	temp_src = (unsigned char *) src;
	if (!temp_dest && !temp_src)
		return (NULL);
	while (cont < n)
	{
		temp_dest[cont] = temp_src[cont];
		cont++;
	}
	return (dest);
}
