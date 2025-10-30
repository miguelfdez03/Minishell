/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 19:18:31 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/30 19:10:46 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_pipes(int i, t_data **data)
{
	t_cmd	*cmd;
	
	init_cmd(cmd);
	(*data)->pipe_flag = (*data)->pipe_flag + 1;
	add_to_token(&((*data)->tokens), PIPE, ft_strdup("|"));
	(* data)->cmd->next = cmd;
	
	return (1);
}
