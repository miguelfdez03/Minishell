/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 15:53:47 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/02/25 18:25:09 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printptr_pf(unsigned long long ptr)
{
	int	count;

	count = 0;
	if (!ptr)
		return (ft_printstr_pf("(nil)"));
	count += ft_printstr_pf("0x");
	count += ft_printhexa_pf(ptr, 'x');
	return (count);
}

int	ft_printdec_pf(long long int n)
{
	char	res;
	int		counter;

	counter = 0;
	if (n < 0)
	{
		n = -n;
		counter += ft_printchar_pf('-');
	}
	if (n > 9)
	{
		counter += ft_printdec_pf(n / 10);
		if (counter == -1)
			return (-1);
	}
	res = n % 10 + '0';
	counter += ft_printchar_pf(res);
	if (counter == -1)
		return (-1);
	return (counter);
}

int	ft_printunsigned_pf(unsigned long long n)
{
	int	count;
	int	aux;

	count = 0;
	if (n <= 9)
	{
		aux = ft_printchar_pf(n + '0');
		if (aux == -1)
			return (-1);
		count += aux;
	}
	else
	{
		aux = ft_printunsigned_pf(n / 10);
		if (aux == -1)
			return (-1);
		count += aux;
		aux = ft_printunsigned_pf(n % 10);
		if (aux == -1)
			return (-1);
		count += aux;
	}
	return (count);
}
