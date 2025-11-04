/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 17:11:51 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/04 23:15:40 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_syntax(t_data **data)
{
	t_token	*tokens;

	tokens = (*data)->tokens;
	t_token	*tokens;

	tokens = (*data)->tokens;
	if (tokens->type == PIPE)
		return (EXIT_FAILURE);
}