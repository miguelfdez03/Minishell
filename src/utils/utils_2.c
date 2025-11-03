/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:07:45 by miguel-f          #+#    #+#             */
/*   Updated: 2025/11/03 10:33:08 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_word_length(char *line, int i)
{
	while ((ft_isalpha(line[i]) == 1 || line[i] == 46
			|| line[i] == '$' || line[i] == '_' || ft_isdigit(line[i]) == 1
			|| line[i] == '?' || line[i] == '{' || line[i] == '}'
			|| line[i] == '/' || line[i] == '=')
		&& is_space(line[i]) != EXIT_SUCCESS)
		i++;
	return (i);
}

int	ft_strcmp2(char *str1, char *str2)
{
	int	i;

	if (!str1 || !str2)
		return (-1);
	i = 0;
	while (str1[i] && str2[i] && str1[i] == str2[i])
		i++;
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}

int	had_space_before(char *line, int i)
{
	if (i == 0)
		return (0);
	if (i > 0 && is_space(line[i - 1]) == EXIT_SUCCESS)
		return (1);
	return (0);
}
