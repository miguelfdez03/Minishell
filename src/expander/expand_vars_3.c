/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:41 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:21:48 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: expand_exit_status
 * --------------------------
 * Expande $? con el código de salida del último comando.
 * 
 * Proceso:
 * 1. Convierte el exit_status a string con ft_itoa()
 * 2. Añade ese string al resultado
 * 3. Libera el string temporal
 * 4. Avanza el índice 2 posiciones (para saltar "$?")
 * 
 * Ejemplo: Si exit_status=0, "echo $?" -> "echo 0"
 * 
 * result: Resultado acumulado
 * exit_status: Código de salida del último comando
 * i: Puntero al índice (se actualiza)
 * 
 * Retorna: Nuevo resultado con el exit status añadido
 */
char	*expand_exit_status(char *result, int exit_status, int *i)
{
	char	*status_str;
	char	*new_result;

	status_str = ft_itoa(exit_status);
	new_result = append_to_result(result, status_str);
	free(status_str);
	*i += 2;
	return (new_result);
}

/*
 * Función: expand_pid
 * -----------------
 * Expande $$ con el PID (Process ID) del shell.
 * 
 * Proceso:
 * 1. Obtiene el PID con getpid()
 * 2. Lo convierte a string con ft_itoa()
 * 3. Añade ese string al resultado
 * 4. Libera el string temporal
 * 5. Avanza el índice 2 posiciones (para saltar "$$")
 * 
 * Ejemplo: "echo $$" -> "echo 12345" (donde 12345 es el PID)
 * 
 * result: Resultado acumulado
 * i: Puntero al índice (se actualiza)
 * 
 * Retorna: Nuevo resultado con el PID añadido
 */
char	*expand_pid(char *result, int *i)
{
	char	*pid_str;
	char	*new_result;

	pid_str = ft_itoa(getpid());
	new_result = append_to_result(result, pid_str);
	free(pid_str);
	*i += 2;
	return (new_result);
}
