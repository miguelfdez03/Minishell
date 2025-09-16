/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:50:38 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/11 09:40:21 by lruiz-to         ###   ########.fr       */
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
		return (EXIT_FAILURE);
	else
		return (EXIT_SUCCESS);
}

int	is_quotes(char c)
{
	if (c == '\"' || c == '\'')
		return (EXIT_FAILURE);
	else
		return (EXIT_SUCCESS);
}
