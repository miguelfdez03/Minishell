/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:51:49 by miguel-f          #+#    #+#             */
/*   Updated: 2025/11/04 23:09:55 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	if (arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static void	print_tokens_echo(t_token *tok, int *first)
{
	while (tok && tok->type != PIPE)
	{
		if (tok->type == WORD || tok->type == STRING
			|| tok->type == ARGS || tok->type == SIMPLE_Q)
		{
			if (!(*first) && tok->space)
				ft_putchar_fd(' ', 1);
			ft_putstr_fd(tok->value, 1);
			*first = 0;
		}
		tok = tok->next;
	}
}

int	builtin_echo(t_data *data)
{
	t_token	*tok;
	int		print_newline;
	int		first;

	tok = data->tokens;
	print_newline = 1;
	first = 1;
	if (tok && (ft_strcmp2(tok->value, "echo") == 0))
		tok = tok->next;
	while (tok && is_valid_n_flag(tok->value))
	{
		print_newline = 0;
		tok = tok->next;
	}
	print_tokens_echo(tok, &first);
	if (print_newline)
		ft_putchar_fd('\n', 1);
	return (0);
}
