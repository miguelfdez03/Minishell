/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 18:39:40 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/03 16:01:48 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*temp;
	size_t	cont;

	cont = 0;
	temp = (char *) malloc (ft_strlen(s1) + 1);
	if (!temp)
		return (NULL);
	while (s1[cont])
	{
		temp[cont] = s1[cont];
		cont++;
	}
	temp[cont] = '\0';
	return (temp);
}
