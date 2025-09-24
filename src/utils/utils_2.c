/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 11:30:25 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/24 11:57:54 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_word_length(char *line, int i)
{
	while ((ft_isalpha(line[i]) == 1 || line[i] == 46) && is_space(line[i]) != EXIT_SUCCESS)
		i++;
	return (i);
}

int	ft_redir_length(char *line, int i, t_token_type type)
{
	if (type == 2 || type == 3 || type == 4)
		i++;
	else if (type == 5 || type == 6)
		i = i + 2;
	else
		return (-1);
	while ((ft_isalpha(line[i]) == 1 || line[i] == 46) && is_space(line[i]) != EXIT_SUCCESS)
		i++;
	return (i);
}