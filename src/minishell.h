/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 20:12:56 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/22 11:44:30 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <stdio.h>

typedef enum e_token_type
{
	WORD,
	STRING,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

//--UTILS--
int		is_space(char c);
int		is_quotes(char c);
int		is_symbols(char c);
int		ft_strcmp(char *str1, char *str2, int i);
void	add_to_token(t_token *token, t_token_type type, char *value);
int		ft_word_length(char *line, int i);
//--PARSER CHECKING--
//--LEXER--
int 	lexer(char *line);
int		handle_quotes(char *line, int i, t_token *tokens);
int		check_for_closed(char *line, int i, char quote);
int		check_redir(char *line, int i, t_token *tokens);
int		handle_words(char *line, int i, t_token *tokens);

//--MINI_INIT--
int		main_loop(int argc, char **argv, char **env);

#endif