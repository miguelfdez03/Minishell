/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:21:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/19 11:06:46 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main_loop(int argc, char **argv, char **env)
{
	char *input;
	
	input = NULL;
	while (1)
	{
		input = readline("spidershell>");
		if (!input)
		{
			ft_printf("EXIT");
			break;
		}
		if (ft_strlen(input) > 0)
		{
			add_history(input);
			lexer(input);
		}
	}
}