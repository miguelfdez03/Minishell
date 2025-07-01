/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:22:14 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/08 18:38:31 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	int_len(long nbr)
{
	int	cont;

	cont = 1;
	if (nbr < 0)
	{
		nbr = -nbr;
		cont++;
	}
	while (nbr >= 10)
	{
		nbr /= 10;
		cont++;
	}
	return (cont);
}

static char	*save_str(long n, int len, int neg, char *res)
{
	res[len] = '\0';
	len--;
	if (neg == 1)
	{
		res[0] = '-';
		n *= -1;
	}
	while (len >= neg)
	{
		res[len] = (n % 10) + 48;
		n /= 10;
		len--;
	}
	return (res);
}

static char	*pre_conv(int len)
{
	char	*tmp;

	tmp = (char *)malloc((len + 1) * sizeof(char));
	if (!tmp)
		return (NULL);
	return (tmp);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*res;
	long	nbr;

	nbr = n;
	len = int_len(n);
	res = pre_conv(len);
	if (!res)
		return (NULL);
	if (n < 0)
		res = save_str(nbr, len, 1, res);
	else
		res = save_str(nbr, len, 0, res);
	return (res);
}
