/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:01:14 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/02 23:30:36 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	cont;

	cont = 0;
	while (s[cont])
	{
		if (s[cont] == (char)c)
			return ((char *) &s[cont]);
		cont++;
	}
	if (s[cont] == (char)c)
		return ((char *) &s[cont]);
	return (NULL);
}
