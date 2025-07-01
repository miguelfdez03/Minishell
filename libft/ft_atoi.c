/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:22:14 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/09 16:45:40 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	res;
	int	sign;
	int	cont;

	res = 0;
	cont = 0;
	sign = 1;
	while (str[cont] == 32 || (str[cont] >= 9 && str[cont] <= 13))
		cont++;
	if (str[cont] == '+' && str[cont +1] != '-')
		cont++;
	if (str[cont] == '-')
	{
		sign = sign * -1;
		cont++;
	}
	while (ft_isdigit(str[cont]))
	{
		res = (res * 10) + (str[cont] - 48);
		cont++;
	}
	return (res * sign);
}
