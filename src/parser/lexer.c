/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:28:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/21 16:43:21 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//int tokenizer()
//{
//	
//}

static	int	check_and_exp(t_data **data)
{
	if ((*data)->tokens)
	{
		expand_variables((*data)->tokens, (*data)->env, (*data)->exit_status);
		if ((*data)->tokens->value)
		{
			if ((*data)->cmd->name)
				free((*data)->cmd->name);
			(*data)->cmd->name = ft_strdup((*data)->tokens->value);
			if (create_cmd((*data)->cmd->name, (*data)->cmd) == 0)
				return(EXIT_FAILURE);
			ft_printf("Command assigned: %s\n", (*data)->cmd->name);
		}
	}
	else
		return (EXIT_FAILURE);
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
	ft_printf("--- Tokens before expansion ---\n");
	t_token *tmp = (*data)->tokens;
	while (tmp)
	{
		ft_printf("Token: type=%d, value=%s\n", tmp->type, tmp->value);
		tmp = tmp->next;
	}
	check_and_exp(data);
	ft_printf("--- Tokens after expansion ---\n");
	tmp = (*data)->tokens;
	while (tmp)
	{
		ft_printf("Token: type=%d, value=%s\n", tmp->type, tmp->value);
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}
