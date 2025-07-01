/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:49:18 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/03 00:00:48 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*temp_s;
	size_t			i;
	unsigned char	temp_c;

	i = 0;
	temp_c = (unsigned char ) c;
	temp_s = (unsigned char *) s;
	while (i < n)
	{
		if (temp_s[i] == temp_c)
			return ((void *) &temp_s[i]);
		i++;
	}
	return (NULL);
}
