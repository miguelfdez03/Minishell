/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 13:05:45 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/21 13:14:53 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_exit_status(char *result, int exit_status, int *i)
{
	char	*status_str;
	char	*new_result;

	status_str = ft_itoa(exit_status);
	new_result = append_to_result(result, status_str);
	free(status_str);
	*i += 2;
	return (new_result);
}

char	*expand_pid(char *result, int *i)
{
	char	*pid_str;
	char	*new_result;

	pid_str = ft_itoa(getpid());
	new_result = append_to_result(result, pid_str);
	free(pid_str);
	*i += 2;
	return (new_result);
}
