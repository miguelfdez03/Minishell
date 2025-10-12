z/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:06:17 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/03 13:28:15 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

int	builtin_env(t_data *data)
{
	t_env *current;

	if (!data || !data->env)
	{
		ft_putendl_fd("env: environment not initialized", 2);
		return (1);
	}

	// Recorrer la lista enlazada de variables de entorno
	current = data->env;
	while (current)
	{
		if (current->key && current->value)
			ft_printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}

	return (0);
}
