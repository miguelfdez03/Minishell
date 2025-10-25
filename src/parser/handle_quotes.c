/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 09:27:12 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/25 18:50:16 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_for_closed(char *line, int i, char quote)
{
	i++;
	while (line[i] && line[i] != quote)
		i++;
	if (!line[i])
		return (-1);
	return (i);
}

char	check_unclosed_quotes(char *line)
{
	int		i;
	char	quote;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			quote = line[i];
			i++;
			while (line[i] && line[i] != quote)
				i++;
			if (!line[i])
				return (quote);
			i++;
		}
		else
			i++;
	}
	return (0);
}

int	handle_quotes(char *line, int i, t_data **data)
{
	char	*str;
	int		last;

	last = check_for_closed(line, i, line[i]);
	if (last == -1)
		return (-2);
	str = ft_substr(line, i + 1, last - i - 1);
	if (!str)
		return (-1);
	if (line[i] == '\'')
		add_to_token(&((*data)->tokens), SIMPLE_Q, str);
	else
		add_to_token(&((*data)->tokens), STRING, str);
	return (last + 1);
}
