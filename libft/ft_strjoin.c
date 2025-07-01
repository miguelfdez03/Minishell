/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:17:40 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/03 17:56:26 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*temp;
	size_t	cont;
	size_t	cont2;

	cont = 0;
	cont2 = 0;
	temp = (char *) s1;
	temp = (char *)malloc (((ft_strlen(s1) + ft_strlen(s2) + 1))
			* sizeof(char));
	if (!temp)
		return (NULL);
	while (cont < (ft_strlen(s1)))
		temp[cont2++] = s1[cont++];
	cont = 0;
	while (s2[cont])
	{
		temp[cont2] = s2[cont];
		cont2++;
		cont++;
	}
	temp[cont2] = '\0';
	return (temp);
}
