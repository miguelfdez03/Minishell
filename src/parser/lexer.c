/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:28:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/20 09:30:56 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	lexer(char *line, t_data **data)
{
	int	i;
	int	result;

	i = 0;
	while (line[i])
	{
		if (is_space(line[i]) == EXIT_SUCCESS)
			i++;
		else if (is_quotes(line[i]) == EXIT_SUCCESS)
		{
			result = handle_quotes(line, i, data);
			if (result == -1)
				return (EXIT_FAILURE);
			i = result;
		}
		else if (is_symbols(line[i]) == EXIT_SUCCESS)
		{
			result = check_redir(line, i, data);
			i += result;
		}
		else if (ft_isalpha(line[i]) == 1 || line[i] == '$')
		{
			result = handle_words(line, i, data);
			if (result == -1)
				return (EXIT_FAILURE);
			i = result;
		}
		else
			i++;
	}
	expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);
	return (EXIT_SUCCESS);
}
