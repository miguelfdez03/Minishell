/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/10/02 14:16:25 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Input user → lexer → Tokenizer → Expansión → Crear lista de comandos
** → Ejecutar comandos
*/

int	main(int argc, char **argv, char **env)
{
	t_env   *env_t;
    t_data	*data;
	
	env_t = NULL;
	data = NULL;
	if (init_data(&data, env, env_t) == -1)
	{
		ft_printf("Error: Failed to initialize data\n");
		return (-1);
	}
	main_loop(argc, argv, &data, env);
	return (0);
}