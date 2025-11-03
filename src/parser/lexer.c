/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:28:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/03 10:29:41 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_first_cmd_args(t_token **tmp, t_data **data)
{
	while (*tmp && (*tmp)->type != PIPE)
	{
		if ((*tmp)->value && ((*tmp)->type == WORD || (*tmp)->type == STRING
				|| (*tmp)->type == ARGS || (*tmp)->type == SIMPLE_Q))
			add_cmd_arg((*data)->cmd, (*tmp)->value);
		*tmp = (*tmp)->next;
	}
}

static int	check_and_exp(t_data **data)
{
	t_token	*tmp;

	if (!(*data)->tokens)
		return (EXIT_FAILURE);
	expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);
	concatenate_tokens(&(*data)->tokens);
	tmp = (*data)->tokens;
	if (init_first_cmd(data, &tmp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	process_first_cmd_args(&tmp, data);
	if (tmp && tmp->type == PIPE)
	{
		if (process_pipes(*data) == -1)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	handle_words_and_args(char *line, int i, t_data **data,
	int has_space)
{
	int	result;

	if (ft_isalnum(line[i]) == 1 || line[i] == '$'
		|| line[i] == '.' || line[i] == '/')
	{
		result = handle_words(line, i, data);
		if (result > i)
			set_token_space((*data)->tokens, has_space);
		return (result);
	}
	else if (line[i] == '-')
	{
		result = handle_args(line, i + 1, data);
		if (result > i)
			set_token_space((*data)->tokens, has_space);
		return (result);
	}
	return (-1);
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
	check_and_exp(data);
	return (i);
}
