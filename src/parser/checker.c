/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:01:39 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/21 17:14:17 by lruiz-to         ###   ########.fr       */
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

int	handle_quotes(char *line, int i, t_data **data)
{
	char	*str;
	int		last;

	last = check_for_closed(line, i, line[i]);
	if (last == -1)
	{
		//handle the not closed case
		return (-1);
	}
	str = ft_substr(line, i + 1, last - i - 1);
	ft_printf("%s\n", str);
	if (!str)
		return (-1);
	add_to_token(&((*data)->tokens), STRING, str);
	return (last + 1);
}

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
		(*data)->pipe_flag = 1;
		add_to_token(&((*data)->tokens), PIPE, ft_strdup("|"));
		return (1);
	}
	return (0);
}
