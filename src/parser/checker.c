/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:57 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/21 08:41:58 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_redir(char *line, int i, t_data **data, t_token_type type)
{
	int		start;
	char	quote;

	i++;
	while (is_space(line[i]) == EXIT_SUCCESS)
		i++;
	start = i;
	while (line[i] && is_symbols(line[i]) == EXIT_FAILURE)
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			quote = line[i++];
			while (line[i] && line[i] != quote)
				i++;
			if (line[i] == quote)
				i++;
		}
		else if (is_space(line[i]) == EXIT_SUCCESS)
			break ;
		else
			i++;
	}
	add_to_token(&((*data)->tokens), type, ft_substr(line, start, i - start));
	return (i);
}

int	check_redir(char *line, int i, t_data **data)
{
	if (line[i] == '<' && line[i + 1] == '<')
		return (handle_redir(line, i + 1, data, HEREDOC) - i);
	else if (line[i] == '>' && line[i + 1] == '>')
		return (handle_redir(line, i + 1, data, REDIR_APPEND) - i);
	else if (line[i] == '>' && line[i + 1] != '>')
		return (handle_redir(line, i, data, REDIR_OUT) - i);
	else if (line[i] == '<' && line[i + 1] != '<')
		return (handle_redir(line, i, data, REDIR_IN) - i);
	else if (line[i] == '|')
	{
		add_to_token(&((*data)->tokens), PIPE, ft_strdup("|"));
		return (1);
	}
	return (0);
}
