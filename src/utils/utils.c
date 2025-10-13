/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:08:08 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/13 10:42:09 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_to_token(t_token **tokens, t_token_type type, char *value)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->type = type;
	new_token->value = value;
	new_token->next = NULL;
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

int	is_space(char c)
{
	if (c == 32 || c == '\t')
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
	if (c == '>' || c == '<' || c == '|')
		return (EXIT_SUCCESS);
	else
		return (EXIT_FAILURE);
}

int	ft_strcmp(char *str1, char *str2, int i)
{
	size_t	c;

	c = 0;
	while (str1[i] && str2[c])
	{
		if (str1[i] != str2[c])
			return ((unsigned char)str1[i] - (unsigned char)str2[c]);
		c++;
		i++;
	}
	return (0);
}
