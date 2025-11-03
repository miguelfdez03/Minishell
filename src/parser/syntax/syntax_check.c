/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 17:11:51 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/03 11:13:14 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_syntax(t_data **data)
{
	t_token	*tokens;

	tokens = (*data)->tokens;
	if (tokens->type == PIPE)
		return (EXIT_FAILURE);
}
