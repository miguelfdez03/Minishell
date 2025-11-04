/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 17:11:51 by lruiz-to          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/11/03 22:55:31 by miguel-f         ###   ########.fr       */
=======
/*   Updated: 2025/11/03 22:35:32 by miguel-f         ###   ########.fr       */
>>>>>>> origin/executor
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_syntax(t_data **data)
{
	t_token	*tokens;

	tokens = (*data)->tokens;
<<<<<<< HEAD
	t_token	*tokens;

	tokens = (*data)->tokens;
	if (tokens->type == PIPE)
		return (EXIT_FAILURE);
}

=======
	if (tokens->type == PIPE)
		return (EXIT_FAILURE);
}
>>>>>>> origin/executor
