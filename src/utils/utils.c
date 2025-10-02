/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:08:08 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/02 12:08:10 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_to_token(t_token *token, t_token_type type, char *value)
{
	token = malloc(sizeof(t_token));
	init_tokens(token);
	token->type = type;
	token->value = value;
	ft_printf("%d\n", token->type);
	ft_printf("%s\n", token->value);
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
