/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:28:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/02 15:56:21 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int lexer(char *line, t_token **tokens)
{
	int			i;
	int			result;

	i = 0;
	*tokens = NULL;
	while (line[i])
	{
		if (is_space(line[i]) == EXIT_SUCCESS)
			i++;
		else if (is_quotes(line[i]) == EXIT_SUCCESS)
		{
			result = handle_quotes(line, i, tokens);
			if (result == -1)
				return (EXIT_FAILURE);
			i = result;
		}
		else if (is_symbols(line[i]) == EXIT_SUCCESS)
		{
			result = check_redir(line, i, tokens);
			if (result == 0)
				i++;
			else
				i += result;
		}
		else if (ft_isalpha(line[i]) == 1)
		{
			result = handle_words(line, i, tokens);
			if (result == -1)
				return (EXIT_FAILURE);
			i = result;
		}
		else
			i++;
	}
	return (EXIT_SUCCESS);
}