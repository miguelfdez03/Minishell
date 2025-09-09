/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 20:12:56 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/09 11:29:30 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>

typedef enum e_token_type
{
	STRING,
	WORD,
	PIPE,
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	HEREDOC
}			t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}				t_token;

#endif