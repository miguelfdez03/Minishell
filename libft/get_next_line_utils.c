/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 11:10:24 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/02/25 18:23:06 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen_gnl(const char *str)
{
	size_t	c;

	if (!str)
		return (0);
	c = 0;
	while (str[c] != '\0')
		c++;
	return (c);
}

char	*ft_strchr_gnl(const char *s, int c)
{
	size_t	cont;

	cont = 0;
	if (!s)
		return (0);
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

void	*ft_calloc_gnl(size_t count, size_t size)
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

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char		*endstr;
	size_t		totlen;
	int			i;
	int			j;

	totlen = ft_strlen_gnl(s1) + ft_strlen_gnl(s2);
	endstr = ft_calloc_gnl(sizeof(char), (totlen + 1));
	if (!endstr)
		return (NULL);
	i = 0;
	while (s1 && s1[i])
	{
		endstr[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j])
		endstr[i++] = s2[j++];
	free(s1);
	return (endstr);
}
