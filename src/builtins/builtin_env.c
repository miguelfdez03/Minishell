/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:06:17 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/03 11:58:11 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

int	builtin_env(t_cmd *cmd, t_data *data)
{
	(void)cmd;   // Por ahora no usamos cmd
	(void)data;  // Por ahora no usamos data - cuando lo uses, accede a data->env
	
	// Implementación temporal: necesitarás acceder a las variables desde data->env
	ft_printf("builtin env - pendiente de implementar correctamente con t_data\n");
	return (0);
}
