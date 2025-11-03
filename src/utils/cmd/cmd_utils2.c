/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 16:01:15 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/03 09:22:16 by lruiz-to         ###   ########.fr       */
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
