/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:07:45 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/20 15:39:59 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_word_length(char *line, int i)
{
	while ((ft_isalpha(line[i]) == 1 || line[i] == 46 
		|| line[i] == '$' || line[i] == '_' || ft_isdigit(line[i]) == 1
		|| line[i] == '?' || line[i] == '{' || line[i] == '}')
		&& is_space(line[i]) != EXIT_SUCCESS)
		i++;
	ft_printf("%d, numero",i);
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

t_cmd	*parse_simple_input(char *input)
{
	char	**tokens;
	t_cmd	*cmd;
	int		i;

	// Split simple por espacios
	tokens = ft_split(input, ' ');
	if (!tokens || !tokens[0])
		return (NULL);
	// Crear comando con el primer token
	cmd = create_cmd(tokens[0]);
	if (!cmd)
	{
		free_string_array(tokens);
		return (NULL);
	}
	// Añadir argumentos
	i = 0;
	while (tokens[i])
	{
		add_cmd_arg(cmd, tokens[i]);
		i++;
	}
	free_string_array(tokens);
	return (cmd);
}
