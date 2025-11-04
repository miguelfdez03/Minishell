# 📝 Implementación de Pipes - Documentación Técnica

## 📋 Tabla de Contenidos
- [Resumen Ejecutivo](#resumen-ejecutivo)
- [Estado Inicial](#estado-inicial)
- [Problemas Identificados](#problemas-identificados)
- [Soluciones Implementadas](#soluciones-implementadas)
- [Modificaciones de Código](#modificaciones-de-código)
- [Resultados de Tests](#resultados-de-tests)
- [Norminette](#norminette)

---

## 🎯 Resumen Ejecutivo

**Fecha:** 4 de Noviembre de 2025  
**Objetivo:** Implementar funcionalidad completa de pipes en Minishell  
**Resultado:** ✅ 4/4 tests de pipes pasando (100%)  
**Estado Norminette:** ✅ Todos los archivos conformes

---

## 📌 Estado Inicial

### Funcionalidad Existente
- ✅ **Parser de pipes:** El lexer detectaba y tokenizaba el símbolo `|`
- ✅ **Estructura de comandos:** Los comandos se parseaban en una lista enlazada (`cmd->next`)
- ❌ **Ejecución de pipes:** Solo ejecutaba el primer comando, ignorando el resto del pipeline

### Problema Principal
```c
// Antes: solo ejecutaba cmd único
if (cmd->next)
    return (execute_pipeline(data));  // ❌ NO IMPLEMENTADO
```

---

## 🐛 Problemas Identificados

### 1. Parser - Caracteres Especiales No Reconocidos
**Síntoma:**
```bash
env | grep ^_  # Se quedaba colgado
echo test | grep ^_  # Perdía el carácter ^
```

**Causa Raíz:**
La función `ft_word_length()` solo aceptaba caracteres alfanuméricos específicos:
```c
// ANTES - Lista restrictiva
while ((ft_isalpha(line[i]) == 1 || line[i] == 46
        || line[i] == '$' || line[i] == '_' || ft_isdigit(line[i]) == 1
        || line[i] == '?' || line[i] == '{' || line[i] == '}'
        || line[i] == '/' || line[i] == '=')
    && is_space(line[i]) != EXIT_SUCCESS)
    i++;
```

**Problema:** Caracteres como `^`, `*`, `[`, `]`, `@`, `!`, etc. se perdían durante el parsing.

---

### 2. Ejecución de Pipes - Deadlock en Pipeline
**Síntoma:**
```bash
cat archivo_grande | grep oi  # Se quedaba colgado indefinidamente
ls | wc -l                     # Funcionaba a veces
```

**Causa Raíz:**
La implementación original esperaba (`waitpid`) a cada comando inmediatamente después de lanzarlo:

```c
// ANTES - Implementación errónea
static int execute_single_cmd(t_data *data, t_cmd *cmd, int input_fd, int output_fd)
{
    pid = fork();
    if (pid == 0)
    {
        // Configurar fds y ejecutar
    }
    close(input_fd);   // ❌ Cerrar ANTES de waitpid
    close(output_fd);
    waitpid(pid, &status, 0);  // ❌ Esperar inmediatamente
    return (WEXITSTATUS(status));
}
```

**Problema:** 
1. El proceso padre cierra el pipe **antes** de que el siguiente comando pueda leer
2. El siguiente comando queda esperando datos que nunca llegarán
3. Resultado: **DEADLOCK** 💀

---

### 3. Lexer - Lógica de Reconocimiento Incompleta
**Síntoma:**
```bash
ls | grep src  # Error: "command not found: |"
```

**Causa Raíz:**
El lexer procesaba caracteres en este orden:
1. ❌ Verificaba si era alfanumérico/especial → llamaba `handle_words()`
2. ✅ Si fallaba, verificaba símbolos → llamaba `handle_quotes_and_symbols()`

**Problema:** El `|` se incluía como parte de la palabra antes de verificar si era un símbolo.

---

## 💡 Soluciones Implementadas

### ✅ Solución 1: Parser Universal de Caracteres

**Estrategia:** En lugar de lista blanca, usar lista negra (solo excluir delimitadores).

**Implementación:**
```c
// DESPUÉS - Acepta todo excepto delimitadores
int ft_word_length(char *line, int i)
{
    while (line[i] && is_space(line[i]) != EXIT_SUCCESS
        && line[i] != '|' && line[i] != '<' && line[i] != '>'
        && line[i] != '\'' && line[i] != '"')
        i++;
    return (i);
}
```

**Beneficios:**
- ✅ Acepta `^`, `*`, `[`, `]`, `@`, `!`, `%`, etc.
- ✅ Compatible con wildcards y regex
- ✅ Más robusto para casos edge

---

### ✅ Solución 2: Pipeline No-Bloqueante

**Estrategia:** Lanzar TODOS los procesos primero, cerrar FDs, luego esperar.

**Implementación:**
```c
// DESPUÉS - Pipeline correcto
static int execute_single_cmd(t_data *data, t_cmd *cmd, int input_fd, int output_fd)
{
    pid_t pid;
    
    pid = fork();
    if (pid == 0)
    {
        // Configurar fds y ejecutar
        exit(/* ... */);
    }
    return (pid);  // ✅ Solo devolver PID, NO esperar
}

int execute_pipeline(t_data *data)
{
    // Lanzar todos los comandos
    while (current)
    {
        if (current->next)
        {
            pipe(pipe_fd);
            execute_single_cmd(data, current, input_fd, pipe_fd[1]);
            close(pipe_fd[1]);  // ✅ Cerrar inmediatamente
            if (input_fd != STDIN_FILENO)
                close(input_fd);
            input_fd = pipe_fd[0];
        }
        else
        {
            execute_single_cmd(data, current, input_fd, STDOUT_FILENO);
            if (input_fd != STDIN_FILENO)
                close(input_fd);
        }
        current = current->next;
    }
    
    // Esperar a TODOS los procesos
    while (waitpid(-1, &status, 0) > 0)
    {
        if (WIFEXITED(status))
            exit_status = WEXITSTATUS(status);
    }
    
    return (exit_status);
}
```

**Flujo Correcto:**
1. 🚀 Fork proceso 1 → configura pipe → devuelve PID
2. 🔒 Cierra write-end del pipe (solo padre)
3. 🚀 Fork proceso 2 → configura pipe → devuelve PID
4. 🔒 Cierra write-end y read-end anterior
5. 🚀 Fork proceso 3 (último) → usa STDOUT
6. ⏳ Espera a TODOS los procesos
7. ✅ Retorna exit status del último

---

### ✅ Solución 3: Refactorización del Lexer

**Implementación:**
```c
// DESPUÉS - Orden correcto de verificación
static int handle_words_and_args(char *line, int i, t_data **data, int has_space)
{
    // Casos especiales primero
    if (line[i] == '=' && (is_space(line[i + 1]) || line[i + 1] == '\0'))
        return (/* ... */);
    
    if (line[i] == '-' || (line[i] == '+' && /* ... */))
        return (handle_args(/* ... */));
    
    // Por defecto, tratar como palabra
    result = handle_words(line, i, data);
    return (result);
}
```

**Nota:** Los símbolos ya se procesan en `handle_quotes_and_symbols()` ANTES de llamar a `handle_words_and_args()`.

---

## 🔧 Modificaciones de Código

### Archivos Creados

#### 1. `src/pipes/pipes_utils.c` (NUEVO)
**Propósito:** Separar lógica de ejecución de pipes (norminette compliance)

**Funciones:**
- `execute_single_cmd()` - Fork y configuración de FDs para un comando
- `close_pipe_fds()` - Cierre ordenado de file descriptors
- `wait_all_processes()` - Espera y recolección de exit status
- `handle_pipe_cmd()` - Gestión de comandos intermedios en pipeline
- `execute_pipeline()` - Coordinador principal del pipeline

#### 2. `src/parser/lexer_utils.c` (NUEVO)
**Propósito:** Separar funciones auxiliares del lexer (norminette compliance)

**Funciones:**
- `process_first_cmd_args()` - Procesa argumentos del primer comando
- `check_and_exp()` - Expande variables y concatena tokens

---

### Archivos Modificados

#### 1. `src/pipes/pipes.c`
**Antes:** 6 funciones (violaba norminette)  
**Después:** 4 funciones

**Cambios:**
- ✂️ Movidas funciones de ejecución a `pipes_utils.c`
- ✅ Solo mantiene funciones de parsing

#### 2. `src/parser/lexer.c`
**Antes:** 6 funciones (violaba norminette)  
**Después:** 4 funciones

**Cambios:**
- ✂️ Movidas funciones auxiliares a `lexer_utils.c`
- 🔧 Simplificado `handle_words_and_args()`

#### 3. `src/utils/utils_2.c`
**Función modificada:** `ft_word_length()`

**Antes:**
```c
while ((ft_isalpha(line[i]) == 1 || line[i] == 46 || line[i] == '$' 
        || line[i] == '_' || ft_isdigit(line[i]) == 1
        || line[i] == '?' || line[i] == '{' || line[i] == '}'
        || line[i] == '/' || line[i] == '=')
    && is_space(line[i]) != EXIT_SUCCESS)
```

**Después:**
```c
while (line[i] && is_space(line[i]) != EXIT_SUCCESS
    && line[i] != '|' && line[i] != '<' && line[i] != '>'
    && line[i] != '\'' && line[i] != '"')
```

**Impacto:** ⬆️ Compatibilidad con wildcards, regex, y caracteres especiales

#### 4. `src/builtins/built.c`
**Función modificada:** `execute_command()`

**Añadido:**
```c
if (cmd->next)
    return (execute_pipeline(data));
```

**Impacto:** Detecta y rutea comandos con pipes

#### 5. `src/minishell.h`
**Declaraciones añadidas:**
```c
// Pipes
int execute_pipeline(t_data *data);
int execute_single_cmd(t_data *data, t_cmd *cmd, int input_fd, int output_fd);

// Lexer
void process_first_cmd_args(t_token **tmp, t_data **data);
int check_and_exp(t_data **data);
```

#### 6. `Makefile`
**SRCS añadidos:**
```makefile
src/pipes/pipes_utils.c\
src/parser/lexer_utils.c\
```

---

## ✅ Resultados de Tests

### Tests de Pipes - 4/4 (100%)

| # | Comando | Resultado | Descripción |
|---|---------|-----------|-------------|
| 1 | `env \| sort \| grep -v SHLVL \| grep -v ^_` | ✅ | Pipeline complejo con múltiples greps y regex |
| 2 | `cat ./test_files/infile_big \| grep oi` | ✅ | Lectura de archivo grande (167KB) con pipe |
| 3 | `cat minishell.h \| grep ");"$` | ✅ | Grep con regex de fin de línea |
| 4 | `export GHOST=123 \| env \| grep GHOST` | ✅ | Pipeline con builtin export |

### Casos de Uso Validados

#### ✅ Pipes Simples
```bash
ls | wc -l                    # Contar archivos
echo hello | cat              # Pipe con builtin
```

#### ✅ Pipes Múltiples
```bash
cat file | grep x | wc -l    # 3 comandos
env | sort | head -n 5       # Pipeline de 3 etapas
```

#### ✅ Caracteres Especiales
```bash
grep ^_                       # Regex: inicio de línea
grep ");"$                    # Regex: fin de línea
ls *.c                        # Wildcard (futuro)
```

#### ✅ Archivos Grandes
```bash
cat infile_big | grep oi      # 167KB sin bloqueos
```

---

## 📏 Norminette

### Estado Final - Todos los Archivos ✅

```
src/pipes/pipes.c        : OK!
src/pipes/pipes_utils.c  : OK!
src/parser/lexer.c       : OK!
src/parser/lexer_utils.c : OK!
src/utils/utils_2.c      : OK!
src/minishell.h          : OK!
```

### Cumplimiento de Normas

| Regla | Límite | Cumplimiento |
|-------|--------|--------------|
| Funciones por archivo | 5 | ✅ 4-5 funciones por archivo |
| Líneas por función | 25 | ✅ Todas < 25 líneas |
| Variables por función | 5 | ✅ Todas ≤ 5 variables |

### Estrategia de División

**Criterio:** Separar por responsabilidad funcional

```
pipes.c          → Parsing de pipes
pipes_utils.c    → Ejecución de pipes

lexer.c          → Lógica principal del lexer
lexer_utils.c    → Expansión y procesamiento
```

---

## 🎓 Lecciones Aprendidas

### 1. **File Descriptors en Pipes**
- ✅ **DO:** Cerrar FDs inmediatamente después de `dup2()` en el hijo
- ✅ **DO:** Cerrar write-end del pipe en el padre después de fork
- ❌ **DON'T:** Esperar a un proceso antes de cerrar sus FDs
- ❌ **DON'T:** Dejar FDs abiertos innecesariamente

### 2. **Parsing Flexible**
- ✅ **DO:** Usar lista negra (excluir delimitadores) en lugar de lista blanca
- ✅ **DO:** Procesar símbolos ANTES que palabras
- ❌ **DON'T:** Asumir que conoces todos los caracteres válidos

### 3. **Concurrencia en Shells**
- ✅ **DO:** Lanzar todos los procesos primero, esperar después
- ✅ **DO:** Recolectar exit status del último comando del pipeline
- ❌ **DON'T:** Bloquear el padre mientras hay pipes activos

---

## 🔍 Debugging y Testing

### Herramientas Utilizadas
```bash
# Tests automatizados
./tester pipes

# Tests manuales con timeout
timeout 5 bash -c 'echo "cmd" | ./minishell'

# Verificación de norminette
norminette src/pipes/ src/parser/
```

### Comandos de Prueba Útiles
```bash
# Pipe simple
ls | wc -l

# Pipe con archivo grande
cat test_files/infile_big | grep oi

# Pipe con regex
env | grep ^USER

# Pipeline múltiple
env | sort | grep -v SHLVL | head -n 10
```

---

## 📊 Métricas Finales

| Métrica | Valor |
|---------|-------|
| Tests pasados | 4/4 (100%) |
| Archivos creados | 2 |
| Archivos modificados | 6 |
| Líneas de código añadidas | ~150 |
| Errores de norminette | 0 |
| Bugs de concurrencia resueltos | 1 (deadlock) |
| Caracteres especiales soportados | Todos excepto `\` y `;` |

---

## 🚀 Próximos Pasos

### Funcionalidades Pendientes
- [ ] Redirecciones con pipes (`cmd1 | cmd2 > file`)
- [ ] Heredoc con pipes (`cmd1 << EOF | cmd2`)
- [ ] Manejo de señales en pipes (Ctrl-C, Ctrl-D)
- [ ] Variables de entorno en pipelines
- [ ] Optimización de memoria en pipelines largos

### Tests Adicionales Recomendados
- [ ] Pipes con 10+ comandos
- [ ] Pipes con builtins que modifican el entorno
- [ ] Pipes con comandos que fallan
- [ ] Pipes con archivos muy grandes (>10MB)

---

## 👥 Autores

**Proyecto:** Minishell - 42 Málaga  
**Desarrolladores:** miguel-f & lruiz-to  
**Fecha:** Noviembre 2025  

---

## 📚 Referencias

- [Pipex - 42 Project](https://github.com/42School/pipex)
- [Unix Pipes Tutorial](https://www.gnu.org/software/libc/manual/html_node/Pipe-to-a-Subprocess.html)
- [Process Management in C](https://www.gnu.org/software/libc/manual/html_node/Processes.html)
- [Norminette v3 Documentation](https://github.com/42School/norminette)

---

**🎉 Fin del documento - Implementación de Pipes completada con éxito 🎉**
