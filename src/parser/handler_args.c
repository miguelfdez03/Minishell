/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 11:27:28 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/03 09:19:26 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_words(char *line, int i, t_data **data)
{
	int		last;
	char	*str;

	while (is_space(line[i]) == EXIT_SUCCESS)
		i++;
	last = ft_word_length(line, i);
	str = ft_substr(line, i, last - i);
	if (!str)
		return (-1);
	add_to_token(&((*data)->tokens), WORD, str);
	return (last);
}

int	handle_args(char *line, int i, t_data **data)
{
	int		last;
	char	*str;

	while (is_space(line[i]) == EXIT_SUCCESS)
		return (-1);
	last = ft_word_length(line, i);
	str = ft_substr(line, i - 1, last - i + 1);
	if (!str)
		return (-1);
	add_to_token(&((*data)->tokens), ARGS, str);
	return (last);
}

int	handle_quotes_and_symbols(char *line, int i, t_data **data,
	int has_space)
{
	int	result;

	if (is_quotes(line[i]) == EXIT_SUCCESS)
	{
		result = handle_quotes(line, i, data);
		if (result > i)
			set_token_space((*data)->tokens, has_space);
		return (result);
	}
	else if (is_symbols(line[i]) == EXIT_SUCCESS)
	{
		result = check_redir(line, i, data);
		if (result > 0)
			set_token_space((*data)->tokens, has_space);
		return (i + result);
	}
	return (-1);
}
