/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:14 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:14:31 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: builtin_pwd
 * ------------------
 * Implementa el comando pwd (print working directory).
 * 
 * 1. Obtiene el directorio actual usando getcwd()
 * 2. Si falla (NULL), muestra mensaje de error
 * 3. Si tiene éxito, imprime el directorio completo
 * 4. Libera la memoria asignada por getcwd
 * 
 * data: Estructura principal del minishell (no se usa)
 * 
 * Retorna: 0 si tiene éxito, 1 si falla
 */
int	builtin_pwd(t_data *data)
{
	char	*pwd;

	(void)data;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_putendl_fd("pwd: error getting current directory", 2);
		return (1);
	}
	ft_putendl_fd(pwd, 1);
	free(pwd);
	return (0);
}
