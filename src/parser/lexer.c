/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:28:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/25 18:33:34 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	process_tokens(t_token *tmp, t_data **data)
{
	while (tmp)
	{
		if (tmp->value && (tmp->type == WORD || tmp->type == STRING
				|| tmp->type == ARGS || tmp->type == SIMPLE_Q))
			add_cmd_arg((*data)->cmd, tmp->value);
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}

static int	check_and_exp(t_data **data)
{
	t_token	*tmp;

	if (!(*data)->tokens)
		return (EXIT_FAILURE);
	expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);
	tmp = (*data)->tokens;
	if (tmp && tmp->value)
	{
		if ((*data)->cmd->name)
			free((*data)->cmd->name);
		(*data)->cmd->name = ft_strdup(tmp->value);
		if (create_cmd((*data)->cmd->name, (*data)->cmd) == 0)
			return (EXIT_FAILURE);
		tmp = tmp->next;
	}
	return (process_tokens(tmp, data));
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
