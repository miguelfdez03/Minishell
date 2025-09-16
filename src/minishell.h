/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel <miguel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 20:12:56 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/16 17:38:22 by miguel           ###   ########.fr       */
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
# include <unistd.h>
# include <sys/wait.h>

// Tipos de tokens que puede reconocer nuestro Spider-Shell
typedef enum e_token_type
{
    WORD,           // Comando o argumento normal (ej: "ls", "-la", "file.txt")
    STRING,         // Cadena entre comillas (ej: "hello world", 'file name')
    PIPE,           // Operador pipe "|" para conectar comandos
    REDIR_IN,       // Redirección de entrada "<" (leer desde archivo)
    REDIR_OUT,      // Redirección de salida ">" (escribir a archivo)
    REDIR_APPEND,   // Redirección append ">>" (añadir al final del archivo)
    HEREDOC         // Here document "<<" (entrada multilínea hasta delimitador)
}   t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

//--UTILS--
int	is_space(char c);
int	is_quotes(char c);

//--PARSER CHECKING--
int	handle_quotes(char *line, int i, t_token *tokens);
int	check_for_closed(char *line, int i);
int	check_redir(char *line, int i, t_token *tokens);

//--LEXER--
int	lexer(char *line);

//--EXECUTOR-- (empezamos aquí)
int test_executor(void);
int test_simple_command(char *cmd_path, char **args);
#endif