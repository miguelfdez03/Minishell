/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:31:33 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/07 16:30:32 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*temp;
	size_t			cont;

	cont = 0;
	temp = malloc(count * size);
	if (!temp)
		return (NULL);
	while (cont < count * size)
	{
		temp[cont] = 0;
		cont++;
	}
	return ((void *) temp);
}
