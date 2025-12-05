/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:43 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:21:48 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: expand_braced_var
 * ------------------------
 * Expande variables con llaves: ${VAR}
 * 
 * Proceso:
 * 1. Busca la llave de cierre }
 * 2. Si no encuentra }: añade "${" literalmente y avanza
 * 3. Si encuentra }:
 *    - Extrae el nombre entre llaves
 *    - Busca su valor en env
 *    - Añade el valor al resultado
 *    - Avanza el índice pasando toda la expresión
 * 
 * Ejemplo: "${HOME}" -> busca HOME y la reemplaza con su valor
 * 
 * str: String completo
 * i: Puntero al índice actual (se actualiza)
 * env: Variables de entorno
 * result: Resultado acumulado
 * 
 * Retorna: Nuevo resultado con la variable expandida
 */
char	*expand_braced_var(char *str, int *i, t_env *env, char *result)
{
	int		j;
	int		len;
	char	*var_name;
	char	*value;
	char	*new_result;

	j = *i + 2;
	len = 0;
	while (str[j + len] && str[j + len] != '}')
		len++;
	if (str[j + len] != '}')
	{
		new_result = append_to_result(result, "${");
		*i += 2;
		return (new_result);
	}
	var_name = ft_substr(str, j, len);
	value = get_var_value(var_name, env);
	free(var_name);
	new_result = append_to_result(result, value);
	*i += len + 3;
	return (new_result);
}

/*
 * Función: expand_simple_var
 * ------------------------
 * Expande variables simples: $VAR
 * 
 * Proceso:
 * 1. Extrae el nombre de la variable (hasta encontrar carácter inválido)
 * 2. Si no hay nombre válido: añade '$' literal
 * 3. Si hay nombre:
 *    - Busca el valor en env
 *    - Añade el valor al resultado
 *    - Avanza el índice
 * 
 * Ejemplo: "$HOME" -> busca HOME y la reemplaza
 *          "$" -> añade '$' literal
 * 
 * str: String completo
 * i: Puntero al índice actual (se actualiza)
 * env: Variables de entorno
 * result: Resultado acumulado
 * 
 * Retorna: Nuevo resultado con la variable expandida
 */
char	*expand_simple_var(char *str, int *i, t_env *env, char *result)
{
	char	*var_name;
	char	*value;
	char	*new_result;
	int		len;

	var_name = extract_var_name(str, *i + 1, &len);
	if (!var_name || len == 0)
	{
		new_result = append_char(result, '$');
		*i += 1;
		return (new_result);
	}
	value = get_var_value(var_name, env);
	free(var_name);
	new_result = append_to_result(result, value);
	*i += len + 1;
	return (new_result);
}

/*
 * Función: handle_dollar
 * --------------------
 * Maneja la expansión de $ según lo que le sigue.
 * 
 * Casos:
 * 1. $? -> exit status del último comando
 * 2. $$ -> PID del shell
 * 3. ${ -> variable con llaves ${VAR}
 * 4. $VAR -> variable simple
 * 5. $ seguido de carácter inválido -> $ literal
 * 
 * str: String completo
 * i: Puntero al índice del $ (se actualiza)
 * env: Variables de entorno
 * exit_status: Código de salida del último comando
 * 
 * Retorna: String con la expansión correspondiente
 */
char	*handle_dollar(char *str, int *i, t_env *env, int exit_status)
{
	char	*result;

	result = NULL;
	if (str[*i + 1] == '?')
		return (expand_exit_status(result, exit_status, i));
	else if (str[*i + 1] == '$')
		return (expand_pid(result, i));
	else if (str[*i + 1] == '{')
		return (expand_braced_var(str, i, env, result));
	else if (is_valid_var_char(str[*i + 1], 1))
		return (expand_simple_var(str, i, env, result));
	else
	{
		result = append_char(result, '$');
		*i += 1;
		return (result);
	}
}

/*
 * Función: expand_string
 * --------------------
 * Expande todas las variables en un string.
 * 
 * Recorre el string carácter por carácter:
 * - Si encuentra $: llama a handle_dollar() para expandir
 * - Si no: añade el carácter literal
 * 
 * Ejemplo:
 * "echo $HOME/file" -> "echo /home/user/file"
 * "exit $?" -> "exit 0"
 * 
 * str: String con posibles variables
 * env: Variables de entorno
 * exit_status: Código de salida para $?
 * 
 * Retorna: String con todas las variables expandidas
 */
char	*expand_string(char *str, t_env *env, int exit_status)
{
	char	*result;
	char	*expansion;
	int		i;

	if (!str)
		return (NULL);
	result = NULL;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			expansion = handle_dollar(str, &i, env, exit_status);
			result = append_to_result(result, expansion);
			free(expansion);
		}
		else
		{
			result = append_char(result, str[i]);
			i++;
		}
	}
	return (result);
}

/*
 * Función: expand_variables
 * -----------------------
 * Expande variables en todos los tokens de la lista.
 * 
 * Proceso:
 * 1. Recorre todos los tokens
 * 2. Salta tokens de comillas simples (no se expanden)
 * 3. Para tokens WORD o STRING:
 *    - Expande todas las variables
 *    - Reemplaza el valor del token
 *    - Si queda vacío: marca como EMPTY
 * 
 * Esto se hace DESPUÉS de procesar comillas, para que
 * las variables en comillas dobles sí se expandan pero
 * las de comillas simples no.
 * 
 * tokens: Lista de tokens a procesar
 * env: Variables de entorno
 * exit_status: Código de salida para $?
 */
void	expand_variables(t_token *tokens, t_env *env, int exit_status)
{
	t_token	*current;
	char	*expanded;

	current = tokens;
	while (current)
	{
		if (current->type == SIMPLE_Q)
		{
			current = current->next;
			continue ;
		}
		if ((current->type == WORD || current->type == STRING)
			&& current->value)
		{
			expanded = expand_string(current->value, env, exit_status);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
				if (expanded[0] == '\0')
					current->type = EMPTY;
			}
		}
		current = current->next;
	}
}
