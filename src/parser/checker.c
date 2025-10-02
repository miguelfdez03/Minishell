/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:01:39 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/02 16:27:15 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//TODO : check for single quotes
//TODO : check args
int	check_for_closed(char *line, int i, char quote)
{
	i++;
	while (line[i] && line[i] != quote)
		i++;
	if (!line[i])
		return (-1);
	return (i);
}

int	handle_quotes(char *line, int i, t_token **tokens)
{
	char	*str;
	int		last;
	
	last = check_for_closed(line, i, line[i]);
	if (last == -1)
	{
		printf("Error: quote not closed\n");
		return (-1);
	}
	str = ft_substr(line, i, last - i + 1);
	if (!str)
	{
		printf("Error: memory allocation failed\n");
		return (-1);
	}
	add_to_token(tokens, STRING, str);
	return (last + 1);
}

int	check_redir(char *line, int i, t_token **tokens)
{
	if (line[i] == '<' && line[i + 1] == '<')
	{
		add_to_token(tokens, HEREDOC, ft_strdup("<<"));
		return (2);
	}
	else if (line[i] == '>' && line[i + 1] == '>')
	{
		add_to_token(tokens, REDIR_APPEND, ft_strdup(">>"));
		return (2);
	}
	else if (line[i] == '>' && line[i + 1] != '>')
	{
		add_to_token(tokens, REDIR_OUT, ft_strdup(">"));
		return (1);
	}
	else if (line[i] == '<' && line[i + 1] != '<')
	{
		add_to_token(tokens, REDIR_IN, ft_strdup("<"));
		return (1);
	}
	else if (line[i] == '|')
	{
		add_to_token(tokens, PIPE, ft_strdup("|"));
		return (1);
	}
}
