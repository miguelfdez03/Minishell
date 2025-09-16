/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel <miguel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:04:22 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/16 14:45:03 by miguel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check for empty
// lexer->token = parse
// Input user ➜ lexer ➜ Tokenizer ➜ Expansión ➜ Crear lista de comandos ➜ Ejecutar
// ?? history
int	main(void)
{
	char *line;

	// Test del executor
	printf("\n=== TESTING EXECUTOR ===\n");
	test_executor();
	printf("=== END TEST ===\n\n");

	while (1)
	{
		line = readline(">spidershell$> ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		// lexer(line);
	}
}