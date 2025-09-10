/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:01:39 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/10 18:21:08 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//TODO : check for single quotes
//! ---- NOT FINISHED ----
int	check_for_closed(char *line, int i)
{
	int cont;

	cont = 0;
	i++;
	while ((line[i] != '\"' || line[i] != '\'') && line[i] != "\0")
	{
		i++;
		cont++;	
	}
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
		last = check_for_closed(line, i);
		str = ft_substr(line, i, last);
	}
	//add to token
	return (EXIT_SUCCESS);
}

int	check_redir(char *line, int i, t_token *tokens)
{
	if (line[i] == "<" && line[i + 1] == "<")
		//HEREDOC
	if (line[i] == ">" && line[i + 1] == ">")
		//rediur append
	if (line[i] == ">" && line[i + 1] != ">")
		//REDIR_output
	if (line[i] == "<" && line[i + 1] != "<")
		//REDIR_INPUT
	return (EXIT_SUCCESS);
}
