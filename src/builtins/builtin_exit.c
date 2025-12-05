/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:06 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:14:31 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: is_numeric_argument
 * ----------------------------
 * Verifica si una cadena es un número válido para exit.
 * 
 * 1. Comprueba que no sea NULL ni vacía
 * 2. Permite signo opcional al inicio (+ o -)
 * 3. Verifica que todos los caracteres restantes sean dígitos
 * 
 * Acepta: "42", "-1", "+100"
 * Rechaza: "abc", "12a", "-", "+", ""
 * 
 * arg: Cadena a verificar
 * 
 * Retorna: 1 si es numérico, 0 si no lo es
 */
static int	is_numeric_argument(const char *arg)
{
	int	i;

	i = 0;
	if (!arg || !arg[0])
		return (0);
	if (arg[0] == '-' || arg[0] == '+')
	{
		i = 1;
		if (!arg[i])
			return (0);
	}
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
 * Función: check_exit_args
 * ------------------------
 * Valida los argumentos del comando exit y calcula el código de salida.
 * 
 * 1. Cuenta cuántos argumentos hay
 * 2. Si hay más de 1: retorna -1 (error: demasiados argumentos)
 * 3. Si no hay argumentos: retorna 0 (salir con éxito)
 * 4. Si hay 1 argumento:
 *    - Si no es numérico: muestra error y retorna 2
 *    - Si es numérico: convierte a int y aplica & 255 (0-255)
 * 
 * El & 255 asegura que el código esté en rango 0-255 como en bash.
 * 
 * cmd: Estructura del comando con sus argumentos
 * 
 * Retorna: Código de salida (0-255), -1 si hay demasiados args, 2 si no es numérico
 */
static int	check_exit_args(t_cmd *cmd)
{
	int	arg_count;

	arg_count = 0;
	if (cmd->args)
	{
		while (cmd->args[arg_count])
			arg_count++;
	}
	if (arg_count > 1)
		return (-1);
	if (arg_count == 0 || !cmd->args[0])
		return (0);
	if (!is_numeric_argument(cmd->args[0]))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": numeric argument required", 2);
		return (2);
	}
	return (ft_atoi(cmd->args[0]) & 255);
}

/*
 * Función: builtin_exit
 * --------------------
 * Implementa el comando exit del shell.
 * 
 * 1. Imprime "exit" en stderr
 * 2. Valida los argumentos y obtiene el código de salida
 * 3. Si hay demasiados argumentos: muestra error y retorna 1 (NO sale)
 * 4. Si todo está bien:
 *    - Libera toda la memoria del shell
 *    - Cierra todos los file descriptors abiertos (3-1023)
 *    - Sale del programa con el código especificado
 * 
 * Ejemplos:
 *   exit      -> sale con código 0
 *   exit 42   -> sale con código 42
 *   exit abc  -> muestra error y sale con código 2
 *   exit 1 2  -> muestra error y NO sale (retorna 1)
 * 
 * data: Estructura principal del minishell
 * 
 * Retorna: 1 si hay demasiados argumentos (sin salir), nunca retorna en otros casos
 */
int	builtin_exit(t_data *data)
{
	t_cmd	*cmd;
	int		exit_status;
	int		fd;

	cmd = data->cmd;
	ft_putendl_fd("exit", 2);
	exit_status = check_exit_args(cmd);
	if (exit_status == -1)
	{
		ft_putendl_fd("exit: too many arguments", 2);
		return (1);
	}
	free_data(data);
	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
	exit(exit_status);
}
