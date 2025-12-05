/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:44 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:21:48 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: is_valid_var_char
 * ------------------------
 * Verifica si un carácter es válido en un nombre de variable.
 * 
 * Reglas:
 * - Primer carácter (is_first=1): solo letras (a-z, A-Z) o guion bajo (_)
 * - Caracteres siguientes: letras, números (0-9) o guion bajo (_)
 * 
 * Ejemplos válidos: VAR, _var, var123, MY_VAR2
 * Inválidos: 123var, -var, var-name
 * 
 * c: Carácter a validar
 * is_first: 1 si es el primer carácter, 0 si no
 * 
 * Retorna: 1 si es válido, 0 si no
 */
int	is_valid_var_char(char c, int is_first)
{
	if (is_first)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
}

/*
 * Función: get_var_value
 * --------------------
 * Busca el valor de una variable en la lista de entorno.
 * 
 * Recorre la lista de variables de entorno hasta encontrar
 * una con el mismo nombre (key) y retorna su valor.
 * 
 * var_name: Nombre de la variable a buscar
 * env: Lista de variables de entorno
 * 
 * Retorna: Valor de la variable o NULL si no existe
 */
char	*get_var_value(char *var_name, t_env *env)
{
	t_env	*current;

	if (!var_name || !env)
		return (NULL);
	current = env;
	while (current)
	{
		if (current->key && ft_strcmp2(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

/*
 * Función: extract_var_name
 * -----------------------
 * Extrae el nombre de una variable desde una posición.
 * 
 * Ejemplo: "$HOME/dir" desde posición 1 -> extrae "HOME"
 * 
 * Proceso:
 * 1. Verifica que el primer carácter sea válido
 * 2. Cuenta cuántos caracteres válidos hay
 * 3. Extrae el substring con el nombre
 * 4. Actualiza len con la longitud del nombre
 * 
 * str: String completo
 * start: Posición inicial (después del $)
 * len: Salida - longitud del nombre extraído
 * 
 * Retorna: Nombre de la variable (debe liberarse) o NULL si inválido
 */
char	*extract_var_name(char *str, int start, int *len)
{
	int		i;
	char	*name;

	i = 0;
	*len = 0;
	if (!str || !is_valid_var_char(str[start], 1))
		return (NULL);
	while (str[start + i] && is_valid_var_char(str[start + i], i == 0))
		i++;
	*len = i;
	name = ft_substr(str, start, i);
	return (name);
}

/*
 * Función: append_to_result
 * -----------------------
 * Añade un string al resultado acumulado.
 * 
 * Casos:
 * 1. to_add es NULL: retorna result sin cambios
 * 2. result es NULL: duplica to_add
 * 3. Ambos existen: concatena result + to_add, libera result viejo
 * 
 * result: String acumulado hasta ahora (se libera si se concatena)
 * to_add: String a añadir (no se libera)
 * 
 * Retorna: Nuevo resultado con to_add añadido
 */
char	*append_to_result(char *result, char *to_add)
{
	char	*new_result;

	if (!to_add)
		return (result);
	if (!result)
		return (ft_strdup(to_add));
	new_result = ft_strjoin(result, to_add);
	free(result);
	return (new_result);
}

/*
 * Función: append_char
 * ------------------
 * Añade un solo carácter al resultado.
 * 
 * Crea un string temporal de 1 carácter y lo añade usando
 * append_to_result().
 * 
 * result: String acumulado
 * c: Carácter a añadir
 * 
 * Retorna: Nuevo resultado con el carácter añadido
 */
char	*append_char(char *result, char c)
{
	char	*new_result;
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	new_result = append_to_result(result, temp);
	return (new_result);
}
