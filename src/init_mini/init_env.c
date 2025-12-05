/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:23:59 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: get_env_key
 * -------------------
 * Extrae el nombre (clave) de una variable de entorno.
 * 
 * Busca hasta el '=' y extrae todo lo anterior.
 * 
 * Ejemplo: "PATH=/usr/bin" -> extrae "PATH"
 * 
 * env: Array de variables de entorno
 * i: Índice de la variable a procesar
 * 
 * Retorna: Nombre de la variable (debe liberarse)
 */
char	*get_env_key(char **env, int i)
{
	char	*key;
	int		j;

	j = 0;
	while (env[i][j] && env[i][j] != '=')
		j++;
	key = ft_substr(env[i], 0, j);
	return (key);
}

/*
 * Función: get_env_value_2
 * -----------------------
 * Extrae el valor de una variable de entorno.
 * 
 * Busca el '=' y extrae todo lo que viene después.
 * Si no hay '=', retorna " " (espacio).
 * 
 * Ejemplo: "PATH=/usr/bin" -> extrae "/usr/bin"
 *          "VARIABLE" -> retorna " "
 * 
 * env: Array de variables de entorno
 * i: Índice de la variable a procesar
 * 
 * Retorna: Valor de la variable (debe liberarse) o " "
 */
char	*get_env_value_2(char **env, int i)
{
	char	*value;
	int		j;
	int		len;

	j = 0;
	while (env[i][j] && env[i][j] != '=')
		j++;
	if (env[i][j] == '=')
	{
		j++;
		len = ft_strlen(env[i]) - j;
		value = ft_substr(env[i], j, len);
	}
	else
		value = " ";
	return (value);
}

/*
 * Función: no_env_init
 * -------------------
 * Inicializa variables de entorno mínimas si no hay env.
 * 
 * Se ejecuta cuando el shell se inicia sin variables de entorno.
 * Crea variables esenciales:
 * 1. PWD: directorio actual con getcwd()
 * 2. SHLVL: nivel de shell = 1
 * 3. PATH: rutas básicas "/bin:/usr/bin"
 * 4. _: "/usr/bin/env"
 * 
 * env_t: Primer nodo de la lista de variables
 * i: Índice inicial
 */
static void	no_env_init(t_env *env_t, int i)
{
	env_t->key = ft_strdup("PWD");
	env_t->value = getcwd(NULL, 0);
	env_t->index = i++;
	env_t->next = malloc(sizeof(t_env));
	if (!env_t->next)
		return ;
	env_t = env_t->next;
	env_t->key = ft_strdup("SHLVL");
	env_t->value = ft_strdup("1");
	env_t->index = i++;
	env_t->next = malloc(sizeof(t_env));
	if (!env_t->next)
		return ;
	env_t = env_t->next;
	env_t->key = ft_strdup("PATH");
	env_t->value = ft_strdup("/bin:/usr/bin");
	env_t->index = i++;
	env_t->next = malloc(sizeof(t_env));
	if (!env_t->next)
		return ;
	env_t = env_t->next;
	env_t->key = ft_strdup("_");
	env_t->value = ft_strdup("/usr/bin/env");
	env_t->index = i;
	env_t->next = NULL;
}

/*
 * Función: init_env
 * ----------------
 * Inicializa la lista de variables de entorno del shell.
 * 
 * Proceso:
 * 1. Si env está vacío: crea entorno mínimo con no_env_init()
 * 2. Si no: recorre env[]
 *    - Para cada variable: extrae key, value e index
 *    - Crea nodos enlazados
 * 3. El último nodo tiene next = NULL
 * 
 * env: Array de variables de entorno del sistema
 * env_t: Primer nodo de la lista a inicializar
 * 
 * Retorna: 0 si éxito, -1 si falla malloc
 */
int	init_env(char **env, t_env *env_t)
{
	int	i;
	int	j;

	i = 0;
	if (!env[0])
		no_env_init(env_t, i);
	while (env[i])
	{
		env_t->key = get_env_key(env, i);
		env_t->value = get_env_value_2(env, i);
		env_t->index = i;
		i++;
		if (env[i])
		{
			env_t->next = malloc(sizeof(t_env));
			if (!env_t->next)
				return (-1);
			env_t = env_t->next;
		}
		else
			env_t->next = NULL;
	}
	return (0);
}

/*
 * Función: increment_shlvl
 * -----------------------
 * Incrementa el nivel de shell (SHLVL) en 1.
 * 
 * SHLVL indica cuántos shells están anidados:
 * - SHLVL=1: shell principal
 * - SHLVL=2: shell dentro de otro shell
 * - etc.
 * 
 * Proceso:
 * 1. Busca la variable SHLVL en env
 * 2. Si existe: convierte a int, incrementa, guarda nuevo valor
 * 3. Si no existe: crea SHLVL=1
 * 
 * env_head: Puntero a la cabeza de la lista de variables
 */
void	increment_shlvl(t_env **env_head)
{
	t_env	*shlvl_node;
	int		current_level;
	char	*new_level;

	shlvl_node = find_env(*env_head, "SHLVL");
	if (shlvl_node && shlvl_node->value)
	{
		current_level = ft_atoi(shlvl_node->value);
		current_level++;
		new_level = ft_itoa(current_level);
		if (new_level)
		{
			free(shlvl_node->value);
			shlvl_node->value = new_level;
		}
	}
	else
	{
		set_env_value(env_head, "SHLVL", "1");
	}
}
