/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:28:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/18 18:48:10 by lruiz-to         ###   ########.fr       */
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
		printf("%d\n", i);
		if (is_space(line[i]) == EXIT_SUCCESS)
			i++;
		if (is_quotes(line[i]) == EXIT_SUCCESS)
			if (check_for_closed == EXIT_SUCCESS)
				handle_quotes(line, i, tokens);
		if (line[i] == "\>" || line[i] == "\<")//TODO > add pipes to the if
		
			check_redir(line, i, tokens);
		//TODO : add commands check
			i++;
	}
}