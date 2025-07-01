/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:14:16 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/03 19:44:49 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*temp;

	i = 0;
	temp = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!temp)
		return (NULL);
	while (i < ft_strlen(s))
	{
		temp[i] = (*f)(i, s[i]);
		i++;
	}
	temp[i] = '\0';
	return (temp);
}
