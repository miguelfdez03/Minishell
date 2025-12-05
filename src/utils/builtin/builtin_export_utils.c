/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:19 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:14:31 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Función: swap_env_nodes
 * ------------------------
 * Intercambia el contenido de dos nodos de variables de entorno.
 * 
 * Intercambia los punteros key y value entre los nodos, sin mover
 * los nodos en la lista. Esto es útil para ordenar la lista.
 * 
 * a: Primer nodo
 * b: Segundo nodo
 */
void	swap_env_nodes(t_env *a, t_env *b)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = a->key;
	temp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = temp_key;
	b->value = temp_value;
}

/*
 * Función: is_valid_identifier
 * ----------------------------
 * Verifica si un nombre es un identificador válido para variables.
 * 
 * Reglas de validación:
 * 1. No puede ser NULL ni vacío
 * 2. Primer carácter: debe ser letra (a-z, A-Z) o guion bajo (_)
 * 3. Caracteres restantes: letras, números (0-9) o guion bajo (_)
 * 
 * Válidos: "VAR", "_var", "var123", "MY_VAR"
 * Inválidos: "123var", "-var", "var-name", "var!name"
 * 
 * name: Cadena a validar
 * 
 * Retorna: 1 si es válido, 0 si no lo es
 */
int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/*
 * Función: extract_key
 * --------------------
 * Extrae el nombre de la variable de un argumento export.
 * 
 * Ejemplo: "VAR=valor" -> extrae "VAR"
 * 
 * 1. Calcula la longitud del nombre (desde inicio hasta el '=')
 * 2. Reserva memoria para el nombre
 * 3. Copia el nombre y añade '\0' al final
 * 
 * arg: Argumento completo
 * equal_pos: Puntero a la posición del carácter '='
 * 
 * Retorna: Nombre de la variable (debe liberarse), NULL si falla malloc
 */
static char	*extract_key(char *arg, char *equal_pos)
{
	int		len;
	char	*key;

	len = equal_pos - arg;
	key = malloc(sizeof(char) * (len + 1));
	if (!key)
		return (NULL);
	ft_strlcpy(key, arg, len + 1);
	return (key);
}

/*
 * Función: extract_value
 * ----------------------
 * Extrae el valor de una variable de un argumento export.
 * 
 * Ejemplo: "VAR=valor" -> extrae "valor"
 *          "VAR="      -> extrae "" (cadena vacía)
 * 
 * 1. Duplica todo lo que viene después del '='
 * 2. Puede ser una cadena vacía si no hay nada después del '='
 * 
 * equal_pos: Puntero a la posición del carácter '='
 * 
 * Retorna: Valor de la variable (debe liberarse), NULL si falla malloc
 */
static char	*extract_value(char *equal_pos)
{
	char	*value;

	value = ft_strdup(equal_pos + 1);
	return (value);
}

/*
 * Función: parse_export_arg
 * -------------------------
 * Parsea un argumento de export separándolo en nombre y valor.
 * 
 * Casos:
 * 1. "VAR=valor"  -> key="VAR", value="valor"
 * 2. "VAR="       -> key="VAR", value="" (cadena vacía)
 * 3. "VAR"        -> key="VAR", value=NULL (solo exportar)
 * 4. "=valor"     -> key="=", value=NULL (error: empieza con =)
 * 
 * La función NO valida si el identificador es válido, solo separa.
 * 
 * arg: Argumento a parsear
 * key: Puntero donde guardar el nombre (salida)
 * value: Puntero donde guardar el valor (salida)
 */
void	parse_export_arg(char *arg, char **key, char **value)
{
	char	*equal_pos;

	*key = NULL;
	*value = NULL;
	if (!arg && !ft_strchr(arg, '='))
		return ;
	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		if (equal_pos == arg)
		{
			*key = ft_strdup("=");
			*value = NULL;
			return ;
		}
		*key = extract_key(arg, equal_pos);
		*value = extract_value(equal_pos);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}
