/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:35:39 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/02 23:36:49 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	cont;

	cont = ft_strlen(s);
	while (cont >= 0 && s[cont] != (char)c)
		cont--;
	if (cont < 0)
		return (NULL);
	return ((char *) &s[cont]);
}
