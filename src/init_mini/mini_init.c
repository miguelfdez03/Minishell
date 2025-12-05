/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:49 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:23:59 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Función: free_data
 * -----------------
 * Libera toda la memoria de la estructura principal.
 * 
 * Libera en orden:
 * 1. Lista de variables de entorno (env)
 * 2. Lista de tokens
 * 3. Estructura de comandos
 * 4. Input (línea leída)
 * 5. Array de rutas de PATH
 * 6. La estructura data en sí
 * 
 * Verifica que cada puntero no sea NULL antes de liberar.
 * 
 * data: Estructura a liberar
 */
void	free_data(t_data *data)
{
	if (!data)
		return ;
	if (data->env)
		free_env(data->env);
	if (data->tokens)
		free_tokens(data->tokens);
	if (data->cmd)
		free_cmd(data->cmd);
	if (data->input)
		free(data->input);
	if (data->path)
		free_string_array(data->path);
	free(data);
}

/*
 * Función: read_multiline
 * ----------------------
 * Lee entrada multi-línea cuando hay comillas sin cerrar.
 * 
 * Proceso:
 * 1. Verifica si hay comillas sin cerrar con check_unclosed_quotes()
 * 2. Mientras haya comillas abiertas:
 *    - Muestra prompt "> " para continuar
 *    - Lee línea adicional
 *    - Si EOF (Ctrl+D): muestra error de sintaxis
 *    - Concatena con newline entre líneas
 * 3. Repite hasta que todas las comillas estén cerradas
 * 
 * Ejemplo: echo "hola<Enter> -> muestra "> " y espera más input
 * 
 * input: Línea inicial
 * data: Estructura para actualizar exit_status si hay error
 * 
 * Retorna: Input completo o NULL si hay error
 */
static char	*read_multiline(char *input, t_data **data)
{
	char	quote;
	char	*cont;
	char	*temp;

	quote = check_unclosed_quotes(input);
	while (quote != 0)
	{
		cont = readline("> ");
		if (!cont)
		{
			ft_putendl_fd("syntax error: unexpected end of file", 2);
			(*data)->exit_status = 2;
			free(input);
			return (NULL);
		}
		temp = ft_strjoin(input, "\n");
		free(input);
		input = ft_strjoin(temp, cont);
		free(temp);
		free(cont);
		quote = check_unclosed_quotes(input);
	}
	return (input);
}

/*
 * Función: process_input
 * --------------------
 * Procesa y ejecuta una línea de entrada.
 * 
 * Proceso:
 * 1. Salta espacios y tabs al inicio
 * 2. Verifica que no sea línea vacía ni comentario (#)
 * 3. Si hay contenido:
 *    - Ejecuta lexer para tokenizar
 *    - Ejecuta el comando
 *    - Libera tokens y cmd
 *    - Reinicializa cmd para siguiente comando
 * 
 * input: Línea a procesar
 * data: Estructura principal del shell
 */
static void	process_input(char *input, t_data **data)
{
	int	i;

	i = 0;
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	if (ft_strlen(input) > 0 && input[i] != '#' && input[i] != '\0')
	{
		if (lexer(input, data) >= 0)
		{
			execute_command(*data);
		}
		if ((*data)->tokens)
		{
			free_tokens((*data)->tokens);
			(*data)->tokens = NULL;
		}
		if ((*data)->cmd)
		{
			free_cmd((*data)->cmd);
			(*data)->cmd = NULL;
		}
		if (init_cmd_data(data) == -1)
			return ;
	}
}

/*
 * Función: main_loop
 * -----------------
 * Bucle principal del shell interactivo.
 * 
 * Ciclo:
 * 1. Muestra prompt "spidershell> "
 * 2. Lee línea con readline
 * 3. Si EOF (Ctrl+D): imprime "exit" y termina
 * 4. Si hubo señal: actualiza exit_status
 * 5. Lee líneas adicionales si hay comillas sin cerrar
 * 6. Añade a historial
 * 7. Procesa y ejecuta el input
 * 8. Libera la línea
 * 9. Repite
 * 
 * argc: Número de argumentos (no usado)
 * argv: Argumentos (no usado)
 * data: Estructura principal del shell
 * 
 * Retorna: exit_status final del shell
 */
int	main_loop(int argc, char **argv, t_data **data)
{
	char	*input;

	while (1)
	{
		input = readline("spidershell> ");
		if (!input)
		{
			ft_printf("exit\n");
			break ;
		}
		if (g_signal_received)
		{
			(*data)->exit_status = g_signal_received;
			g_signal_received = 0;
		}
		input = read_multiline(input, data);
		if (!input)
			continue ;
		add_history(input);
		process_input(input, data);
		free(input);
	}
	return ((*data)->exit_status);
}
