/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:01:39 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/04 18:16:58 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_redir(char *line, int i, t_data **data, t_token_type type)
{
	int	start;
	int	len;

	i++;
	while (is_space(line[i]) == EXIT_SUCCESS)
		i++;
	start = i;
	len = 0;
	while (line[i] && is_space(line[i]) == EXIT_FAILURE
		&& is_symbols(line[i]) == EXIT_FAILURE)
	{
		len++;
		i++;
	}
	if (len > 0)
	{
		if (!(*data)->cmd)
			return (i);
		add_redir(&((*data)->cmd->redirections), type,
			ft_substr(line, start, len));
	}
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
