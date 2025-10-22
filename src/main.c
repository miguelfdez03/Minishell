/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 19:57:20 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/22 22:11:46 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Input user → lexer → Tokenizer → Expansión → Crear lista de comandos
** → Ejecutar comandos
*/

int	main(int argc, char **argv, char **env)
{
	t_env	*env_t;
	t_data	*data;

	if (init_data(&data, env, env_t) == -1)
	{
		ft_printf("Error: Failed to initialize data\n");
		return (-1);
	}
	main_loop(argc, argv, &data);
	// free everything
	return (0);
}
