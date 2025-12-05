/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:21 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:36:53 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Función: init_cmd
 * -----------------
 * Inicializa estructura de comando con valores por defecto.
 * 
 * Establece:
 * - name: NULL
 * - args: NULL
 * - builtin_id: BUILTIN_NONE
 * - redirections: NULL
 * - next: NULL
 * 
 * cmd: Comando a inicializar
 */
void	init_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	cmd->name = NULL;
	cmd->args = NULL;
	cmd->builtin_id = BUILTIN_NONE;
	cmd->redirections = NULL;
	cmd->next = NULL;
}

/*
 * Función: identify_builtin
 * -------------------------
 * Identifica si un comando es un builtin.
 * 
 * Compara el nombre del comando con los builtins:
 * - cd, pwd, exit, echo, env, export, unset
 * 
 * cmd: Nombre del comando
 * 
 * Retorna: Tipo de builtin (BUILTIN_CD, BUILTIN_PWD, etc.)
 *          o BUILTIN_NONE si no es builtin
 */
t_builtin_type	identify_builtin(char *cmd)
{
	if (!cmd)
		return (BUILTIN_NONE);
	if (ft_strcmp2(cmd, "cd") == 0)
		return (BUILTIN_CD);
	if (ft_strcmp2(cmd, "pwd") == 0)
		return (BUILTIN_PWD);
	if (ft_strcmp2(cmd, "exit") == 0)
		return (BUILTIN_EXIT);
	if (ft_strcmp2(cmd, "echo") == 0)
		return (BUILTIN_ECHO);
	if (ft_strcmp2(cmd, "env") == 0)
		return (BUILTIN_ENV);
	if (ft_strcmp2(cmd, "export") == 0)
		return (BUILTIN_EXPORT);
	if (ft_strcmp2(cmd, "unset") == 0)
		return (BUILTIN_UNSET);
	return (BUILTIN_NONE);
}

/*
 * Función: create_cmd
 * ------------------
 * Crea e inicializa un comando con su nombre.
 * 
 * Proceso:
 * 1. Inicializa estructura del comando
 * 2. Duplica el nombre del comando
 * 3. Identifica si es builtin
 * 4. Establece builtin_id apropiado
 * 
 * cmd_name: Nombre del comando
 * cmd: Estructura donde crear el comando
 * 
 * Retorna: 1 si éxito, 0 si falla
 */
int	create_cmd(char *cmd_name, t_cmd *cmd)
{
	t_builtin_type	builtin_id;

	if (!cmd)
		return (0);
	init_cmd(cmd);
	cmd->name = ft_strdup(cmd_name);
	if (!cmd->name)
	{
		free(cmd);
		return (0);
	}
	builtin_id = identify_builtin(cmd_name);
	if (builtin_id != BUILTIN_NONE)
		cmd->builtin_id = builtin_id;
	else
		cmd->builtin_id = BUILTIN_NONE;
	return (1);
}

/*
 * Función: count_cmd_args
 * ----------------------
 * Cuenta cuántos argumentos tiene un comando.
 * 
 * Recorre el array de argumentos hasta encontrar NULL.
 * 
 * cmd: Comando a contar argumentos
 * 
 * Retorna: Número de argumentos
 */
int	count_cmd_args(t_cmd *cmd)
{
	int	count;

	count = 0;
	if (cmd && cmd->args)
	{
		while (cmd->args[count])
			count++;
	}
	return (count);
}

/*
 * Función: add_cmd_arg
 * -------------------
 * Añade un argumento al array de argumentos del comando.
 * 
 * Proceso:
 * 1. Cuenta argumentos actuales
 * 2. Reserva nuevo array con espacio para uno más
 * 3. Copia argumentos existentes
 * 4. Añade nuevo argumento al final
 * 5. Libera array anterior
 * 
 * cmd: Comando donde añadir argumento
 * arg: Argumento a añadir (se duplica)
 */
void	add_cmd_arg(t_cmd *cmd, char *arg)
{
	int		i;
	int		count;
	char	**new_args;

	if (!cmd || !arg)
		return ;
	count = count_cmd_args(cmd);
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
	i = 0;
	if (cmd->args)
	{
		while (i < count)
		{
			new_args[i] = cmd->args[i];
			i++;
		}
		free(cmd->args);
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	cmd->args = new_args;
}
