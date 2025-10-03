/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:07:00 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/03 13:36:10 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_exit_args(t_cmd *cmd);

/**
 * @brief Termina la ejecución del shell con un código de salida específico.
 *
 * Esta función maneja el comando "exit" adaptado a la arquitectura Spider-Shell.
 * Valida argumentos y termina con el código de salida apropiado.
 *
 * @param cmd Estructura de comando con argumentos
 */
int	builtin_exit(t_data *data)
{
	t_cmd *cmd;
	int	exit_status;

	cmd = data->cmd;
	ft_putendl_fd("exit", 2);
	exit_status = check_exit_args(cmd);
	
	if (exit_status == -1) // Error de demasiados argumentos
	{
		ft_putendl_fd("exit: too many arguments", 2);
		return (1); // No sale, solo retorna error
	}
	
	// Salir con el código correspondiente
	exit(exit_status);
}

/**
 * @brief Verifica y procesa los argumentos para el comando "exit".
 *
 * Adaptado a la estructura t_cmd de Spider-Shell.
 *
 * @param cmd Estructura de comando con argumentos
 * @return El código de salida o -1 en caso de demasiados argumentos
 */
static int	check_exit_args(t_cmd *cmd)
{
	int	i;
	int	arg_count;

	// Contar argumentos (args[0] es el comando mismo)
	arg_count = 0;
	if (cmd->args)
	{
		while (cmd->args[arg_count])
			arg_count++;
	}

	// Si hay más de 2 argumentos (exit + 1 argumento max)
	if (arg_count > 2)
		return (-1);
	
	// Si solo hay "exit" sin argumentos
	if (arg_count <= 1 || !cmd->args[1])
		return (0);
	
	// Validar que el argumento sea numérico
	i = 0;
	if (cmd->args[1][0] == '-' || cmd->args[1][0] == '+')
		i = 1; // Permitir signo
	
	while (cmd->args[1][i])
	{
		if (!ft_isdigit(cmd->args[1][i]))
		{
			ft_printf("exit: %s: numeric argument required\n", cmd->args[1]);
			exit(2);
		}
		i++;
	}
	
	return (ft_atoi(cmd->args[1]) & 255); 
}