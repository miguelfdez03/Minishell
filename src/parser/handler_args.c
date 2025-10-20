/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 11:27:28 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/20 14:51:10 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_words(char *line, int i, t_data **data)
{
	int		last;
	char	*str;

	while (is_space(line[i]) == EXIT_SUCCESS)
		i++;
	last = ft_word_length (line, i);
	str = ft_substr (line, i, last - i);
		ft_printf("\n: ",str);
	if (!str)
		return (-1);
	ft_printf("%s\n", str);
	add_to_token(&((*data)->tokens), WORD, str);
	return (last);
}
