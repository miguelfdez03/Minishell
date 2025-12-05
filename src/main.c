/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:44 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:48:26 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: main
 * -------------
 * Punto de entrada del shell minishell.
 * 
 * Proceso:
 * 1. Configura señales para modo interactivo
 * 2. Inicializa estructura de datos del shell
 * 3. Entra en el bucle principal (main_loop)
 * 4. Al salir:
 *    - Limpia historial de readline
 *    - Libera toda la memoria
 *    - Retorna código de salida
 * 
 * argc: Número de argumentos (no usado)
 * argv: Array de argumentos (no usado)
 * env: Variables de entorno del sistema
 * 
 * Retorna: Código de salida del shell
 */
int	main(int argc, char **argv, char **env)
{
	t_env	*env_t;
	t_data	*data;
	int		exit_code;

	setup_signals_interactive();
	if (init_data(&data, env, env_t) == -1)
	{
		ft_printf("Error: Failed to initialize data\n");
		return (-1);
	}
	exit_code = main_loop(argc, argv, &data);
	rl_clear_history();
	free_data(data);
	return (exit_code);
}
