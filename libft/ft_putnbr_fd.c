/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 21:11:48 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/07 20:51:55 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	res;
	int		neg;

	if (n == -2147483648)
		write (fd, "-2147483648", 11);
	else if (n < 0)
	{
		neg = n * -1;
		write (fd, "-", 1);
		ft_putnbr_fd (neg, fd);
	}
	else if (n >= 10)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
	else if (n < 10)
	{
		res = n + 48;
		write(fd, &res, 1);
	}
}
