/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:48 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:23:59 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: init_tokens
 * -------------------
 * Inicializa un token con valores por defecto.
 * 
 * Valores iniciales:
 * - value: NULL (sin contenido)
 * - space: 0 (sin espacio antes)
 * - type: EMPTY (tipo vacío)
 * 
 * token: Token a inicializar
 */
void	init_tokens(t_token *token)
{
	token->value = NULL;
	token->space = 0;
	token->type = EMPTY;
}

/*
 * Función: init_cmd_data
 * ---------------------
 * Inicializa la estructura de comando.
 * 
 * Reserva memoria y establece valores por defecto:
 * - name: NULL (sin nombre)
 * - args: NULL (sin argumentos)
 * - builtin_id: BUILTIN_NONE (no es builtin)
 * - redirections: NULL (sin redirecciones)
 * - next: NULL (sin siguiente comando en pipeline)
 * 
 * data: Puntero a estructura data que contiene el comando
 * 
 * Retorna: 0 si éxito, -1 si falla malloc
 */
int	init_cmd_data(t_data **data)
{
	(*data)->cmd = malloc(sizeof(t_cmd));
	if (!(*data)->cmd)
	{
		ft_printf("Error: Failed to allocate memory for cmd structure\n");
		return (-1);
	}
	(*data)->cmd->name = NULL;
	(*data)->cmd->args = NULL;
	(*data)->cmd->builtin_id = BUILTIN_NONE;
	(*data)->cmd->redirections = NULL;
	(*data)->cmd->next = NULL;
	return (0);
}

/*
 * Función: init_env_struct
 * -----------------------
 * Inicializa la estructura de variables de entorno.
 * 
 * Proceso:
 * 1. Reserva memoria para el primer nodo de env
 * 2. Asigna env_t a data->env
 * 3. Llama a init_env() para copiar variables del sistema
 * 4. Incrementa SHLVL
 * 5. Si falla: libera memoria y retorna error
 * 
 * data: Puntero a estructura principal
 * env: Array de variables de entorno del sistema
 * env_t: Puntero para almacenar el primer nodo
 * 
 * Retorna: 0 si éxito, -1 si error
 */
static int	init_env_struct(t_data **data, char **env, t_env **env_t)
{
	*env_t = malloc(sizeof(t_env));
	if (!*env_t)
	{
		free(*data);
		return (-1);
	}
	(*data)->env = *env_t;
	if (init_env(env, *env_t) == -1)
	{
		free(*env_t);
		free(*data);
		return (-1);
	}
	increment_shlvl(&(*data)->env);
	return (0);
}

/*
 * Función: init_cmd_and_data
 * -------------------------
 * Inicializa el comando y campos adicionales de data.
 * 
 * Inicializa:
 * 1. Estructura cmd con init_cmd_data()
 * 2. path: NULL (sin rutas de PATH aún)
 * 3. pipe_flag: -1 (sin pipes)
 * 4. exit_status: 0 (éxito inicial)
 * 
 * Si falla init_cmd_data: libera memoria y retorna error.
 * 
 * data: Puntero a estructura principal
 * env_t: Nodo de variables (para liberar si falla)
 * 
 * Retorna: 0 si éxito, -1 si error
 */
static int	init_cmd_and_data(t_data **data, t_env *env_t)
{
	if (init_cmd_data(data) == -1)
	{
		free(env_t);
		free(*data);
		return (-1);
	}
	(*data)->path = NULL;
	(*data)->pipe_flag = -1;
	(*data)->exit_status = 0;
	return (0);
}

/*
 * Función: init_data
 * -----------------
 * Inicializa toda la estructura principal del shell.
 * 
 * Proceso completo:
 * 1. Reserva memoria para data
 * 2. Inicializa input y tokens a NULL
 * 3. Inicializa variables de entorno con init_env_struct()
 * 4. Inicializa comando y otros campos con init_cmd_and_data()
 * 
 * Si falla en cualquier paso: libera memoria y retorna error.
 * 
 * data: Puntero a estructura principal a inicializar
 * env: Array de variables de entorno del sistema
 * env_t: Variable temporal para nodos de env
 * 
 * Retorna: 0 si éxito, -1 si error
 */
int	init_data(t_data **data, char **env, t_env *env_t)
{
	*data = malloc(sizeof(t_data));
	if (!*data)
		return (-1);
	(*data)->input = NULL;
	(*data)->tokens = NULL;
	if (init_env_struct(data, env, &env_t) == -1)
		return (-1);
	if (init_cmd_and_data(data, env_t) == -1)
		return (-1);
	return (0);
}
