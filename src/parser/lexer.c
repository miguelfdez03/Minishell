/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:28:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/22 21:47:40 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	check_and_exp(t_data **data)
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
	while (tmp)
	{
		if (tmp->value && (tmp->type == WORD || tmp->type == STRING
				|| tmp->type == ARGS))
			add_cmd_arg((*data)->cmd, tmp->value);
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}

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
			i = handle_quotes(line, i, data);
			if (result == -1)
				return (EXIT_FAILURE);
			i = result;
		}
		else if (is_symbols(line[i]) == EXIT_SUCCESS)
		{
			result = check_redir(line, i, data);
			i += result;
		}
		else if (ft_isalpha(line[i]) == 1 || line[i] == '$' || line[i] == '.' || line[i] == '/')
		{
			result = handle_words(line, i, data);
			if (result == -1)
				return (EXIT_FAILURE);
			i = result;
		}
		else if (line[i] ==  '-')
			i = handle_args(line, i + 1, data);
		//add error case
		else
			i++;
	}
	check_and_exp(data);
	return (EXIT_SUCCESS);
}
