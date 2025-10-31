/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:28:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/31 19:45:32 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	init_first_cmd(t_data **data, t_token **tmp)
{
	if (*tmp && (*tmp)->value)
	{
		if (create_cmd((*tmp)->value, (*data)->cmd) == 0)
			return (EXIT_FAILURE);
		*tmp = (*tmp)->next;
	}
	return (EXIT_SUCCESS);
}

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

static int	handle_lexer_char(char *line, int i, t_data **data)
{
	int	result;

	if (is_space(line[i]) == EXIT_SUCCESS)
		return (i + 1);
	else if (is_quotes(line[i]) == EXIT_SUCCESS)
		return (handle_quotes(line, i, data));
	else if (is_symbols(line[i]) == EXIT_SUCCESS)
	{
	result = check_redir(line, i, data);
		return (i + result);
	}
	else if (ft_isalpha(line[i]) == 1 || line[i] == '$'
		|| line[i] == '.' || line[i] == '/')
		return (handle_words(line, i, data));
	else if (line[i] == '-')
		return (handle_args(line, i + 1, data));
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
