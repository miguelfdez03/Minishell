/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:50:32 by lruiz-to          #+#    #+#             */
/*   Updated: 2024/10/01 20:52:11 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	int	cont;

	cont = 0;
	while (s[cont])
	{
		write(fd, &s[cont], 1);
		cont++;
	}
	write(fd, "\n", 1);
}
