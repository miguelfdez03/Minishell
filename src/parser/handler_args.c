/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 11:27:28 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/02 15:55:08 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_words(char *line, int i, t_token **tokens)
{
	int last;
	char *str;
	
	while (is_space(line[i]) == EXIT_SUCCESS)
		i++;
	last = ft_word_length(line, i);
	str = ft_substr(line, i, last - i);
	if (!str)
		return (-1);
	add_to_token(tokens, WORD, str);
	return (last);
}