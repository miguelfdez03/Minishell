/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:12 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:14:31 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: sort_env_list
 * ----------------------
 * Ordena alfabéticamente la lista de variables de entorno.
 * 
 * Usa el algoritmo de ordenamiento de burbuja (bubble sort):
 * 1. Recorre la lista repetidamente mientras haya intercambios
 * 2. En cada pasada, compara pares de nodos consecutivos
 * 3. Si están en orden incorrecto (strcmp > 0), los intercambia
 * 4. Continúa hasta que no haya más intercambios
 * 
 * Esta ordenación es necesaria para que 'export' muestre las
 * variables en orden alfabético, como lo hace bash.
 * 
 * env: Lista enlazada de variables de entorno a ordenar
 */
static void	sort_env_list(t_env *env)
{
	t_env	*current;
	t_env	*next;
	int		swapped;

	if (!env)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = env;
		while (current && current->next)
		{
			next = current->next;
			if (ft_strcmp2(current->key, next->key) > 0)
			{
				swap_env_nodes(current, next);
				swapped = 1;
			}
			current = current->next;
		}
	}
}

/*
 * Función: export_without_args
 * ----------------------------
 * Muestra todas las variables exportadas en formato declare -x.
 * 
 * 1. Ordena la lista de variables alfabéticamente
 * 2. Recorre todas las variables de entorno
 * 3. Para cada variable:
 *    - Si tiene valor: imprime declare -x KEY="VALUE"
 *    - Si no tiene valor: imprime declare -x KEY (sin comillas)
 * 
 * Este formato imita el comportamiento de bash cuando ejecutas
 * 'export' sin argumentos.
 * 
 * env: Lista de variables de entorno
 * 
 * Retorna: 0 siempre
 */
static int	export_without_args(t_env *env)
{
	t_env	*current;

	if (!env)
		return (0);
	sort_env_list(env);
	current = env;
	while (current)
	{
		if (current->key)
		{
			if (current->value)
				ft_printf("declare -x %s=\"%s\"\n",
					current->key, current->value);
			else
				ft_printf("declare -x %s\n", current->key);
		}
		current = current->next;
	}
	return (0);
}

/*
 * Función: export_with_assignment
 * -------------------------------
 * Exporta una variable al entorno del shell.
 * 
 * 1. Valida que el nombre de la variable sea un identificador válido:
 *    - Debe empezar por letra o '_'
 *    - Solo puede contener letras, números y '_'
 * 2. Si no es válido, muestra error
 * 3. Si es válido, añade o actualiza la variable en el entorno
 * 
 * data: Estructura principal del minishell
 * key: Nombre de la variable
 * value: Valor a asignar (puede ser NULL si solo se exporta sin valor)
 * 
 * Retorna: 0 si tiene éxito, 1 si el identificador no es válido
 */
static int	export_with_assignment(t_data *data, char *key, char *value)
{
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(key, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	set_env_value(&data->env, key, value);
	return (0);
}

/*
 * Función: process_export_arg
 * ---------------------------
 * Procesa un argumento individual del comando export.
 * 
 * 1. Valida que el argumento no empiece por '=' (error de sintaxis)
 * 2. Separa el argumento en key y value:
 *    - Si contiene '=': export VAR=valor
 *    - Si no contiene '=': export VAR (solo exporta, sin valor)
 * 3. Llama a export_with_assignment para añadir la variable
 * 4. Libera la memoria temporal de key y value
 * 
 * data: Estructura principal del minishell
 * arg: Argumento a procesar (ej: "VAR=valor" o "VAR")
 * 
 * Retorna: 0 si tiene éxito, 1 si hay error
 */
static int	process_export_arg(t_data *data, char *arg)
{
	char	*key;
	char	*value;
	int		status;

	if (!arg || arg[0] == '=')
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	parse_export_arg(arg, &key, &value);
	if (!key)
		return (1);
	status = export_with_assignment(data, key, value);
	free(key);
	if (value)
		free(value);
	return (status);
}

/*
 * Función: builtin_export
 * -----------------------
 * Implementa el comando export del shell.
 * 
 * 1. Si no hay argumentos: muestra todas las variables exportadas
 * 2. Si hay argumentos: procesa cada uno:
 *    - export VAR=valor   -> crea/actualiza variable con valor
 *    - export VAR         -> marca variable como exportada sin valor
 *    - export VAR1 VAR2   -> procesa múltiples variables
 * 3. Si alguna variable falla, continúa con las demás
 * 4. Retorna error solo si al menos una falló
 * 
 * data: Estructura principal del minishell
 * 
 * Retorna: 0 si todas las variables se exportaron bien, 1 si alguna falló
 */
int	builtin_export(t_data *data)
{
	int		i;
	int		status;

	if (!data || !data->cmd)
		return (1);
	if (!data->cmd->args || !data->cmd->args[0])
		return (export_without_args(data->env));
	status = 0;
	i = 0;
	while (data->cmd->args[i])
	{
		if (process_export_arg(data, data->cmd->args[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}
