/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:50:38 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/19 11:10:54 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_to_token(t_token *token, t_token_type type, char *value)
{
	token->type = type;
	token->value = value;
	token = token->next;
}

int	is_space(char c)
{
	if (c == 32)
		return (EXIT_SUCCESS);
	else
		return (EXIT_FAILURE);
}

int	is_quotes(char c)
{
	if (c == '\"' || c == '\'')
		return (EXIT_SUCCESS);
	else
		return (EXIT_FAILURE);
}

int	is_symbols(char c)
{
	if (c == '\>' || c == '\<' || c == '\|')
		return (EXIT_SUCCESS);
	else
		return (EXIT_FAILURE);
}
