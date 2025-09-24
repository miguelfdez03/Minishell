/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 11:27:28 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/23 16:24:16 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int handle_redir(char *line, int i, t_token *token)
{
	
}

int	handle_words(char *line, int i, t_token *token)
{
	int last;
	char *str;
	
	last = 0;	
	str = NULL;
	while (is_space(line[i]) == EXIT_SUCCESS)
		i++;
	last = ft_word_length(line, i);
	str = ft_substr(line, i, last - i);
	add_to_token(token, WORD, str);
	return (last);
}