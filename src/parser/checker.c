/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:01:39 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/18 11:21:00 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//TODO : check for single quotes
//! ---- NOT FINISHED ----
int	check_for_closed(char *line, int i, char quote)
{
	while (line[i] != quote && line[i] != "\0")
		i++;
	if (i == strlen(line))
		return (-1);
	else
		return (i);
}

int	handle_quotes(char *line, int i, t_token *tokens)
{
	char	*str;
	int		last;
	
	last = 0;
	str = NULL;
	while (line[i] != '\0')
	{
		last = check_for_closed(line, i++, line[i]);
		str = ft_substr(line, i, last - i);
	}
	printf("handle_quotes");
	return (EXIT_SUCCESS);
}

int	check_redir(char *line, int i, t_token *tokens)
{
	if (line[i] == "<" && line[i + 1] == "<")
		printf("HEREDOC");
	if (line[i] == ">" && line[i + 1] == ">")
		printf("rediur append");
	if (line[i] == ">" && line[i + 1] != ">")
		printf("REDIR_output");
	if (line[i] == "<" && line[i + 1] != "<")
		printf("REDIR_INPUT");
	return (EXIT_SUCCESS);
}
