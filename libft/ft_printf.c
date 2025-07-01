/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:25:54 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/02/06 13:06:40 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printchar_pf(char c)
{
	write (1, &c, 1);
	return (1);
}

int	ft_printstr_pf(char *s)
{
	int	counter;

	counter = 0;
	if (!s)
		return (ft_printstr_pf("(null)"));
	while (s[counter])
	{
		write (1, &s[counter], 1);
		counter++;
	}
	return (counter);
}

int	ft_printhexa_pf(unsigned long long n, char c)
{
	int	count;

	count = 0;
	if (n >= 16)
		count += ft_printhexa_pf(n / 16, c);
	if (c == 'x')
		ft_printchar_pf("0123456789abcdef"[n % 16]);
	else if (c == 'X')
		ft_printchar_pf("0123456789ABCDEF"[n % 16]);
	count++;
	return (count);
}

static int	ft_filter(char const *content, int index, va_list *args)
{
	int	counter;

	counter = 0;
	if (content[index] == 'c')
		counter += ft_printchar_pf(va_arg(*args, int));
	if (content[index] == 's')
		counter += ft_printstr_pf(va_arg(*args, char *));
	if (content[index] == 'p')
		counter += ft_printptr_pf(va_arg(*args, unsigned long long));
	if (content[index] == 'd' || content[index] == 'i')
		counter += ft_printdec_pf(va_arg(*args, int));
	if (content[index] == 'u')
		counter += ft_printunsigned_pf(va_arg(*args, unsigned int));
	if (content[index] == 'x' || content[index] == 'X')
		counter += ft_printhexa_pf(va_arg(*args, unsigned int), content[index]);
	if (content[index] == '%')
		counter += ft_printchar_pf('%');
	return (counter);
}

int	ft_printf(char const *content, ...)
{
	va_list	args;
	int		index;
	int		counter;

	index = 0;
	counter = 0;
	va_start(args, content);
	while (content[index])
	{
		if (content[index] != '%')
			counter += ft_printchar_pf(content[index]);
		else
		{
			index++;
			counter += ft_filter(content, index, &args);
		}
		if (counter == -1)
			return (-1);
		index++;
	}
	va_end (args);
	return (counter);
}
