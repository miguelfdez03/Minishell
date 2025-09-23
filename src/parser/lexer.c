/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:28:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/22 17:52:06 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int lexer(char *line)
{
	int			i;
	t_token 	*token;

	i = 0;
	while (line[i])
	{
		if (is_space(line[i]) == EXIT_SUCCESS)
			i++;
		else if (is_quotes(line[i]) == EXIT_SUCCESS)
		{
			i = handle_quotes(line, i, token);
			if (i == EXIT_FAILURE)
				return (EXIT_FAILURE);
			printf("handle-quotes\n");
		}
		else if (is_symbols(line[i]) == EXIT_SUCCESS)
		{
			check_redir(line, i, token);
			i++;
		}
		else if (ft_isalpha(line[i]) == 1)
			i = handle_words(line, i, token);
		else
			i++;
	}
	return (EXIT_SUCCESS);
}