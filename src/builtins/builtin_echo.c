/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:40:50 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:14:31 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: is_valid_n_flag
 * ------------------------
 * Verifica si un argumento es una flag -n válida para echo.
 * 
 * La flag -n puede tener múltiples 'n' seguidas (ej: -nnn, -nnnn)
 * y todas son equivalentes a -n (no imprimir salto de línea).
 * 
 * 1. Comprueba que el argumento empiece por '-'
 * 2. Verifica que el segundo carácter sea 'n'
 * 3. Valida que todos los caracteres restantes sean también 'n'
 * 
 * arg: Cadena a verificar
 * 
 * Retorna: 1 si es una flag -n válida, 0 si no lo es
 */
static int	is_valid_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	if (arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/*
 * Función: builtin_echo
 * ---------------------
 * Implementa el comando echo del shell.
 * 
 * 1. Procesa las flags -n del inicio (pueden ser múltiples: -n -nnn -n)
 * 2. Si encuentra al menos una flag -n, no imprimirá salto de línea al final
 * 3. Imprime todos los argumentos restantes separados por espacios
 * 4. Si no había flag -n, añade un salto de línea al final
 * 
 * Ejemplos:
 *   echo hola mundo      -> "hola mundo\n"
 *   echo -n hola mundo   -> "hola mundo"
 *   echo -n -nnn hola    -> "hola"
 * 
 * data: Estructura principal con la información del comando
 * 
 * Retorna: 0 siempre (echo no falla)
 */
int	builtin_echo(t_data *data)
{
	int		i;
	int		print_newline;
	int		start;

	if (!data || !data->cmd)
		return (1);
	print_newline = 1;
	i = 0;
	while (data->cmd->args && data->cmd->args[i]
		&& is_valid_n_flag(data->cmd->args[i]))
	{
		print_newline = 0;
		i++;
	}
	start = i;
	while (data->cmd->args && data->cmd->args[i])
	{
		ft_putstr_fd(data->cmd->args[i], 1);
		if (data->cmd->args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (print_newline)
		ft_putchar_fd('\n', 1);
	return (0);
}
