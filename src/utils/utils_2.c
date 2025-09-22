/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 11:30:25 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/22 12:30:46 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_word_length(char *line, int i)
{
	while ((ft_isalpha == 1 || line[i] == 46) && is_space != EXIT_SUCCESS)
		i++;
	return (i);
}