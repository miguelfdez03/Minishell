/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 19:57:20 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/24 19:22:28 by lruiz-to         ###   ########.fr       */
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
	free_data(data);
	return (0);
}
