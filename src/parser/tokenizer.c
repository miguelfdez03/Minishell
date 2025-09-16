/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:28:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/10 18:57:10 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// TODO >> recieve and separate the input in to tokens

int lexer(char *line)
{
	t_token		*tokens;
	int			i;

	i = 0;
	while (line[i] != "\0")
	{
		if (is_space(line[i]) == EXIT_SUCCESS)
			i++;
		if (is_quoutes(line[i]) == EXIT_SUCCESS)
			if (check_for_closed == EXIT_SUCCESS)
				handle_quotes(line, i, tokens);
		if (line[i] == ">" || line[i] == "<")
			check_redir(line, i, tokens);
	}
}