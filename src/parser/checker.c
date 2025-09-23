/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:01:39 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/22 18:44:19 by lruiz-to         ###   ########.fr       */
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
		return ( -1);
	printf("dentro: %d\n", i);
	return (i);
}

int	handle_quotes(char *line, int i, t_token *tokens)
{
	char	*str;
	int		last;
	
	last = 0;
	str = NULL;
	last = check_for_closed(line, i, line[i]);
	if (last == -1)
	{
		printf("Error: quote not closed\n");
		return (EXIT_FAILURE);
	}
	str = ft_substr(line, i, last - i + 1);
	add_to_token(tokens, STRING, str);
	return (last + 1);
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
