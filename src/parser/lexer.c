/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:28:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/22 12:42:14 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int lexer(char *line)
{
	t_token		*tokens;
	int			i;
	
	i = 0;
	while (i <= ft_strlen(line))
	{
		if (is_space(line[i]) == EXIT_SUCCESS)
			i++;
		if (is_quotes(line[i]) == EXIT_SUCCESS)
			handle_quotes(line, i, tokens);
		if (is_symbols(line[i]) == EXIT_SUCCESS)
			check_redir(line, i, tokens);
		if (ft_isalpha(line[i]) == 1)
			handle_words(line, i, tokens);
		i++;
	}
}