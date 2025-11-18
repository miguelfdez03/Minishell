# Implementación de Señales en Minishell

## Descripción General

El archivo `signals.c` gestiona el comportamiento de las señales UNIX (SIGINT y SIGQUIT) en diferentes contextos de ejecución de minishell. Esto es crucial para replicar el comportamiento de bash cuando el usuario presiona Ctrl+C o Ctrl+\.

## Variable Global

```c
volatile sig_atomic_t g_signal_received = 0;
```

- **Tipo**: `sig_atomic_t` garantiza operaciones atómicas (thread-safe)
- **volatile**: Indica al compilador que el valor puede cambiar en cualquier momento (por señales)
- **Propósito**: Almacena el código de salida cuando se recibe SIGINT (130 para Ctrl+C)
- **Uso**: Permite que el main loop detecte interrupciones y actualice el exit status

## Funciones de Configuración

### 1. `setup_signals_interactive()`

**Contexto de uso**: Modo interactivo del shell (esperando input del usuario)

```c
void setup_signals_interactive(void)
{
    signal(SIGINT, handle_sigint);  // Ctrl+C -> manejador personalizado
    signal(SIGQUIT, SIG_IGN);       // Ctrl+\ -> ignorar
}
```

**Comportamiento**:
- **SIGINT (Ctrl+C)**: Usa `handle_sigint` para limpiar la línea y mostrar nuevo prompt
- **SIGQUIT (Ctrl+\)**: Ignorada (SIG_IGN) - bash no hace nada en modo interactivo

**Cuando se activa**: Al inicio del programa y después de ejecutar cada comando

### 2. `setup_signals_child()`

**Contexto de uso**: Procesos hijo (comandos externos ejecutándose)

```c
void setup_signals_child(void)
{
    signal(SIGINT, SIG_DFL);   // Ctrl+C -> comportamiento por defecto
    signal(SIGQUIT, SIG_DFL);  // Ctrl+\ -> comportamiento por defecto
}
```

**Comportamiento**:
- **SIG_DFL**: Restaura el comportamiento por defecto del sistema
- **SIGINT**: Termina el proceso con exit code 130
- **SIGQUIT**: Termina el proceso con core dump y exit code 131

**Cuando se activa**: Inmediatamente después de `fork()` en procesos hijo, antes de `execve()`

**Nota importante**: NO ignora SIGPIPE, permitiendo que los procesos terminen silenciosamente cuando escriben a pipes cerrados (comportamiento estándar de bash)

### 3. `setup_signals_heredoc()`

**Contexto de uso**: Mientras se lee un heredoc (<<)

```c
void setup_signals_heredoc(void)
{
    signal(SIGINT, SIG_DFL);   // Ctrl+C -> termina el heredoc
    signal(SIGQUIT, SIG_IGN);  // Ctrl+\ -> ignorar
}
```

**Comportamiento**:
- **SIGINT**: Cancela la lectura del heredoc y retorna al prompt
- **SIGQUIT**: Ignorada como en modo interactivo

**Cuando se activa**: Al iniciar la lectura de un heredoc, antes de entrar al loop de lectura

### 4. `setup_signals_executing()`

**Contexto de uso**: Proceso padre mientras espera a comandos hijo

```c
void setup_signals_executing(void)
{
    signal(SIGINT, SIG_IGN);   // Ctrl+C -> ignorar en padre
    signal(SIGQUIT, SIG_IGN);  // Ctrl+\ -> ignorar en padre
}
```

**Comportamiento**:
- **Ambas señales ignoradas**: El padre no debe reaccionar a Ctrl+C/Ctrl+\
- Las señales se envían al proceso hijo, que las maneja con su propia configuración
- Previene que el shell principal termine cuando el usuario interrumpe un comando

**Cuando se activa**: Justo antes de `waitpid()` en el proceso padre

## Manejador Personalizado: `handle_sigint()`

```c
void handle_sigint(int sig)
{
    (void)sig;                     // Suprimir warning de parámetro no usado
    g_signal_received = 130;       // Guardar exit status (128 + SIGINT(2))
    write(1, "\n", 1);             // Nueva línea (async-signal-safe)
    rl_on_new_line();              // Notificar a readline
    rl_replace_line("", 0);        // Limpiar buffer de readline
    rl_redisplay();                // Redibujar prompt vacío
}
```

**Detalles técnicos**:
- **`write()` en lugar de `printf()`**: Función async-signal-safe (segura en signal handlers)
- **Exit code 130**: Convención UNIX (128 + número de señal), donde SIGINT = 2
- **Funciones de readline**:
  - `rl_on_new_line()`: Informa que el cursor está en nueva línea
  - `rl_replace_line("")`: Borra el contenido actual del buffer
  - `rl_redisplay()`: Redibuja el prompt limpio

**Resultado visible**: Cuando presionas Ctrl+C:
```bash
minishell$ ls -la^C
minishell$ 
```

## Flujo de Señales por Escenario

### Escenario 1: Usuario presiona Ctrl+C en prompt vacío
```
1. Usuario: Ctrl+C
2. Señal SIGINT → handle_sigint()
3. g_signal_received = 130
4. Limpia línea y redibuja prompt
5. Main loop detecta g_signal_received y actualiza exit_status
```

### Escenario 2: Usuario presiona Ctrl+C durante comando externo
```
1. Parent: setup_signals_executing() → ignora señales
2. Child: setup_signals_child() → comportamiento por defecto
3. Usuario: Ctrl+C
4. Señal SIGINT → enviada a child (grupo de procesos)
5. Child termina con exit code 130
6. Parent hace waitpid() y captura el exit status
7. Parent restaura setup_signals_interactive()
```

### Escenario 3: Pipeline con proceso que cierra pipe (cat | echo)
```
1. Child processes: setup_signals_child() → NO ignora SIGPIPE
2. echo termina rápido, cierra su stdin
3. cat intenta escribir → recibe SIGPIPE
4. SIGPIPE termina cat silenciosamente (SIG_DFL)
5. NO se imprime "Tubería rota"
6. Pipeline retorna exit code del último comando (echo = 0)
```

### Escenario 4: Heredoc interrumpido
```
1. Parent: setup_signals_heredoc()
2. Usuario escribe líneas del heredoc
3. Usuario: Ctrl+C
4. Señal SIGINT con SIG_DFL → termina lectura heredoc
5. Retorna al prompt sin ejecutar el comando
```

## Consideraciones de Diseño

### ¿Por qué no ignorar SIGPIPE en child processes?

**Problema anterior**:
```c
signal(SIGPIPE, SIG_IGN);  // ❌ Causaba errores visibles
```
Resultado: `cat | echo` imprimía "error de escritura: Tubería rota"

**Solución actual**:
```c
// No hacer nada con SIGPIPE → usa SIG_DFL
```
Resultado: El proceso termina silenciosamente cuando escribe a pipe cerrado (comportamiento bash)

### ¿Por qué usar volatile sig_atomic_t?

- **`sig_atomic_t`**: Tipo que garantiza lectura/escritura atómica
- **`volatile`**: Evita optimizaciones del compilador que podrían cachear el valor
- **Seguridad**: Permite comunicación segura entre signal handler y main loop

### ¿Por qué diferentes configuraciones?

Cada contexto tiene requisitos distintos:
- **Interactivo**: Usuario controla el shell → respuesta amigable a Ctrl+C
- **Ejecutando**: Comando controla terminal → señales van al comando
- **Heredoc**: Lectura especial → permitir cancelación limpia
- **Child**: Proceso independiente → comportamiento estándar UNIX

## Testing

### Test 1: Ctrl+C en prompt vacío
```bash
minishell$ ^C
minishell$ echo $?
130
```

### Test 2: Ctrl+C durante comando largo
```bash
minishell$ sleep 10
^C
minishell$ echo $?
130
```

### Test 3: Pipeline con pipe cerrado temprano
```bash
minishell$ cat /etc/passwd | head -1
# Debe completar sin errores "Tubería rota"
minishell$ echo $?
0
```

### Test 4: Ctrl+\ en modo interactivo
```bash
minishell$ ^\
# No hace nada, no imprime nada
minishell$ echo $?
0
```

## Relación con Otros Módulos

- **main.c**: Chequea `g_signal_received` en el loop principal
- **executor.c**: Llama `setup_signals_executing()` antes de waitpid
- **pipes.c**: Los child processes llaman `setup_signals_child()`
- **heredoc.c**: Llama `setup_signals_heredoc()` antes de leer

## Referencias POSIX

- **signal(2)**: Instalación de manejadores de señales
- **SIGINT**: Señal de interrupción (Ctrl+C) = 2
- **SIGQUIT**: Señal de quit (Ctrl+\) = 3
- **SIGPIPE**: Señal de pipe roto = 13
- **SIG_DFL**: Comportamiento por defecto del sistema
- **SIG_IGN**: Ignorar la señal
- **Exit codes**: 128 + signal_number para terminación por señal

## Norma 42 Compliance

✅ Máximo 25 líneas por función
✅ Máximo 5 funciones por archivo (6 en total contando el handler)
✅ Variables globales documentadas y justificadas
✅ No leaks (signal handlers no asignan memoria)
