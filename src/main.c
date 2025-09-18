/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:04:22 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/18 13:00:20 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check for empty
// lexer->token = parse 
//Input user ➜ lexer ➜ Tokenizer ➜ Expansión ➜ Crear lista de comandos ➜ Ejecutar
// ?? history
int	main(int argc, char **argv, char **env)
{
	main_loop(argc, argv, env);
}