/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:01:39 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/09 15:15:18 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//TODO : check for single quotes
//! ---- NOT FINISHED ----
int	check_for_closed(char *str, int i)
{
	i++;
	while ((str[i] != '\"' || str[i] != '\'') && str[i] != "\0")
		i++;
	if (i == ft_strlen(str))
		return (-1);
	else
		return (i);
}

int	check_quotes(char *str, int i)
{
	int new_pos;
	
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\"' || str[i] == '\'')	
		{	
			new_pos = check_for_closed(str, i);
			if(new_pos == -1)
				return(EXIT_FAILURE);
		}
	}
}

int	check_redir(char *str, int i)
{
	if (str[i] == "<" && str[i + 1] == "<")
		//HEREDOC
	if (str[i] == ">" && str[i + 1] == ">")
		//rediur append
	if (str[i] == ">" && str[i + 1] != ">")
		//REDIR_output
	if (str[i] == "<" && str[i + 1] != "<")
		//REDIR_INPUT
	return (EXIT_SUCCESS);
}
