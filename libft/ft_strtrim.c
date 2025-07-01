/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 21:54:57 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/03 18:09:02 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	trim(const char *set, char c)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static char	*wrt_str(const char *s1, size_t start, size_t len)
{
	char	*str;
	size_t	i;

	str = ft_calloc(len + 1, sizeof(char));
	i = 0;
	if (len <= 0 || start >= ft_strlen(s1))
		return (ft_strdup(""));
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = s1[start + i];
		i++;
	}
	return (str);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int	start;
	int	end;

	start = 0;
	end = ft_strlen(s1) - 1;
	if (ft_strlen(s1) == 0)
		return (ft_strdup(""));
	while (trim(set, s1[start]))
		start++;
	while (end >= start && trim(set, s1[end]))
		end--;
	return (wrt_str(s1, start, end - start + 1));
}
