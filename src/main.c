/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel <miguel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:04:22 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/24 14:43:53 by miguel           ###   ########.fr       */
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