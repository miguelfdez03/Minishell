/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:49:38 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/12/05 13:45:05 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

/*
 * Función: handle_sigint
 * ---------------------
 * Manejador de señal SIGINT (Ctrl+C) en modo interactivo.
 * 
 * Proceso:
 * 1. Establece g_signal_received a 130 (exit status)
 * 2. Imprime nueva línea
 * 3. Prepara nueva línea en readline
 * 4. Limpia línea actual
 * 5. Redibuja prompt
 * 
 * Esto permite que Ctrl+C cancele la línea actual
 * y muestre un nuevo prompt sin salir del shell.
 * 
 * sig: Número de señal (no usado)
 */
static void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_received = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
 * Función: handle_sigheredoc
 * -------------------------
 * Manejador de SIGINT (Ctrl+C) durante heredoc.
 * 
 * Proceso:
 * 1. Establece g_signal_received a 130
 * 2. Imprime nueva línea
 * 3. Cierra STDIN para salir del bucle de readline
 * 
 * Esto permite cancelar un heredoc con Ctrl+C.
 * 
 * sig: Número de señal (no usado)
 */
static void	handle_sigheredoc(int sig)
{
	(void)sig;
	g_signal_received = 130;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

/*
 * Función: setup_signals_heredoc
 * -----------------------------
 * Configura señales para modo heredoc.
 * 
 * Proceso:
 * 1. Resetea g_signal_received
 * 2. Configura SIGINT con handle_sigheredoc
 * 3. Ignora SIGQUIT (Ctrl+\)
 * 
 * Se llama antes de leer heredoc para que Ctrl+C
 * cancele el heredoc correctamente.
 */
void	setup_signals_heredoc(void)
{
	struct sigaction	sa;

	g_signal_received = 0;
	sa.sa_handler = handle_sigheredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

/*
 * Función: setup_signals_interactive
 * --------------------------------
 * Configura señales para modo interactivo (prompt).
 * 
 * Proceso:
 * 1. Resetea g_signal_received
 * 2. Configura SIGINT con handle_sigint
 * 3. Establece flag SA_RESTART para reiniciar syscalls
 * 4. Ignora SIGQUIT
 * 
 * Se llama al inicio y después de ejecutar comandos
 * para restaurar comportamiento interactivo del shell.
 */
void	setup_signals_interactive(void)
{
	struct sigaction	sa;

	g_signal_received = 0;
	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
