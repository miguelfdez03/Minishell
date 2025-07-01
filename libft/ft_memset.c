/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 09:32:50 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/09 16:24:52 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*temp_ptr;

	temp_ptr = (unsigned char *) b;
	while (len > 0)
	{
		*(temp_ptr++) = (unsigned char) c;
		len--;
	}
	return (b);
}
