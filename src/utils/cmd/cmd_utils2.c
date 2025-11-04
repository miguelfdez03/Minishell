/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 16:01:15 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/04 23:09:17 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	init_first_cmd(t_data **data, t_token **tmp)
{
	if (*tmp && (*tmp)->value)
	{
		if (create_cmd((*tmp)->value, (*data)->cmd) == 0)
			return (EXIT_FAILURE);
		*tmp = (*tmp)->next;
	}
	return (EXIT_SUCCESS);
}
