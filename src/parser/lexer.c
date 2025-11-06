/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:28:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/06 11:14:34 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	had_space_before(char *line, int i)
{
	if (i == 0)
		return (0);
	if (i > 0 && is_space(line[i - 1]) == EXIT_SUCCESS)
		return (1);
	return (0);
}

static int	handle_words_and_args(char *line, int i, t_data **data,
		int has_space)
{
	int	result;

	if (line[i] == '=' && (is_space(line[i + 1]) == EXIT_SUCCESS || line[i
				+ 1] == '\0'))
	{
		add_to_token(&((*data)->tokens), WORD, ft_strdup("="));
		set_token_space((*data)->tokens, has_space);
		return (i + 1);
	}
	if (line[i] == '-' || (line[i] == '+' && (ft_isdigit(line[i + 1]) || line[i
					+ 1] == '"' || line[i + 1] == '\'')))
	{
		result = handle_args(line, i + 1, data);
		if (result > i)
			set_token_space((*data)->tokens, has_space);
		return (result);
	}
	result = handle_words(line, i, data);
	if (result > i)
		set_token_space((*data)->tokens, has_space);
	return (result);
}

static int	handle_lexer_char(char *line, int i, t_data **data)
{
	int	result;
	int	has_space;

	if (is_space(line[i]) == EXIT_SUCCESS)
		return (i + 1);
	has_space = had_space_before(line, i);
	result = handle_quotes_and_symbols(line, i, data, has_space);
	if (result != -1)
		return (result);
	result = handle_words_and_args(line, i, data, has_space);
	if (result != -1)
		return (result);
	return (i + 1);
}

int	lexer(char *line, t_data **data)
{
	int	i;

	i = 0;
	while (line[i] && i >= 0)
	{
		i = handle_lexer_char(line, i, data);
		if (i == -1)
			return (EXIT_FAILURE);
		if (i == -2)
			break ;
	}
	if (!check_syntax(data))
	{
		(*data)->exit_status = 2;
		return (-1);
	}
	check_and_exp(data);
	return (i);
}
