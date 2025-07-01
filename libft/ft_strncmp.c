/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:23:10 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/14 17:06:33 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(char *str1, char *str2, size_t n)
{
	size_t	c;

	c = 0;
	while (c < n && str1[c] && str2[c])
	{
		if (str1[c] != str2[c])
			return ((unsigned char)str1[c] - (unsigned char)str2[c]);
		c++;
	}
	if (c < n)
		return ((unsigned char)str1[c] - (unsigned char)str2[c]);
	return (0);
}
