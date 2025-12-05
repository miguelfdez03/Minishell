/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:00 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:14:31 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: builtin_env
 * ------------------
 * Implementa el comando env (muestra las variables de entorno).
 * 
 * 1. Verifica que el entorno esté inicializado
 * 2. Comprueba que no se hayan pasado argumentos (env no los acepta)
 * 3. Recorre toda la lista de variables de entorno
 * 4. Imprime cada variable en formato KEY=VALUE
 * 5. SOLO muestra variables que tienen valor asignado
 *    (las exportadas sin valor no aparecen en env)
 * 
 * data: Estructura principal del minishell
 * 
 * Retorna: 0 si tiene éxito, 1 si hay error
 */
int	builtin_env(t_data *data)
{
	t_env	*current;

	if (!data || !data->env)
	{
		ft_putendl_fd("env: environment not initialized", 2);
		return (1);
	}
	if (data->cmd && data->cmd->args && data->cmd->args[0])
	{
		ft_putendl_fd("env: too many arguments", 2);
		return (1);
	}
	current = data->env;
	while (current)
	{
		if (current->key && current->value)
			ft_printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
