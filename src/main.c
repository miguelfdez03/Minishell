/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:04:22 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/10 18:17:08 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check for empty
// lexer->token = parse 
//Input user ➜ Tokenizer ➜ Expansión ➜ Crear lista de comandos ➜ Ejecutar
// ?? history 
int	main(void)
{
	char *line;

	while (1)
	{
		line = readline(">spidershell$> ");
		if(!line)
		{
			printf("exit\n");
			break;
		}
		lexer(line);
	}
}