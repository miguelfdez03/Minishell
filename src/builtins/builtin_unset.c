/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:16 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:14:31 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: process_unset_arg
 * --------------------------
 * Procesa un argumento individual del comando unset.
 * 
 * 1. Valida que el nombre de la variable sea un identificador válido
 * 2. Si no es válido, muestra mensaje de error
 * 3. Si es válido, elimina la variable del entorno
 * 
 * data: Estructura principal del minishell
 * arg: Nombre de la variable a eliminar
 * 
 * Retorna: 0 si tiene éxito, 1 si el identificador no es válido
 */
static int	process_unset_arg(t_data *data, char *arg)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("unset: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	unset_env_var(&data->env, arg);
	return (0);
}

/*
 * Función: builtin_unset
 * ---------------------
 * Implementa el comando unset del shell.
 * 
 * 1. Si no hay argumentos: no hace nada (retorna 0)
 * 2. Si hay argumentos: elimina cada variable del entorno
 * 3. Si alguna variable tiene nombre inválido, muestra error
 *    pero continúa con las demás
 * 4. Retorna error solo si al menos una variable fue inválida
 * 
 * Ejemplos:
 *   unset VAR1 VAR2 VAR3   -> elimina las tres variables
 *   unset                  -> no hace nada
 * 
 * data: Estructura principal del minishell
 * 
 * Retorna: 0 si todas las variables se eliminaron bien, 1 si alguna falló
 */
int	builtin_unset(t_data *data)
{
	int		i;
	int		status;

	if (!data || !data->cmd)
		return (1);
	if (!data->cmd->args || !data->cmd->args[0])
		return (0);
	status = 0;
	i = 0;
	while (data->cmd->args[i])
	{
		if (process_unset_arg(data, data->cmd->args[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}
