/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:01:39 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/22 12:43:45 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//TODO : check for single quotes
//TODO : check args
int	check_for_closed(char *line, int i, char quote)
{
	while (line && line[i] != quote && i <= ft_strlen(line))
		i++;
	if (i == ft_strlen(line))
		return ( -1);
	else
		return (i);
}

int	handle_quotes(char *line, int i, t_token *tokens)
{
	char	*str;
	int		last;
	
	last = 0;
	str = NULL;
	last = check_for_closed(line, i, line[i]);
	str = ft_substr(line, i, last - i - 1);
	add_to_token(tokens, STRING, str);
	return (EXIT_SUCCESS);
}

int	check_redir(char *line, int i, t_token *tokens)
{
	if (line[i] == '\<' && line[i + 1] == '\<')
		add_to_token(tokens, HEREDOC, "");
	if (line[i] == '\>' && line[i + 1] == '\>')
		printf("rediur append");
	if (line[i] == '\>' && line[i + 1] != '\>')
		printf("REDIR_output");
	if (line[i] == '\<' && line[i + 1] != '\<')
		printf("REDIR_INPUT");
	if(line[i] == '\|')
		printf("PIPE");
	return (EXIT_SUCCESS);
}
