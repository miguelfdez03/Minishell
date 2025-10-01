/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:04:22 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/30 17:35:51 by lruiz-to         ###   ########.fr       */
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
    t_data	**data;
	
	init_data(data, env, env_t);
	main_loop(argc, argv, env);
	return (0);
}