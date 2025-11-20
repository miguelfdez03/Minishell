# 🚀 Minishell
---

## 🐚 Descripción

**Minishell** es una implementación simplificada de una shell inspirada en bash. Su objetivo es recrear un entorno interactivo donde el usuario pueda ejecutar comandos, usar pipes, redirecciones, y gestionar variables de entorno, todo desde cero.

---

## 💡 Funcionalidades principales

- ✅ Mostrar un prompt mientras espera comandos
- ✅ Guardar y acceder al historial de comandos
- ✅ Buscar y ejecutar el binario correcto
- ✅ Usar **solo una variable global**
- ✅ No interpretar comillas sin cerrar ni caracteres especiales innecesarios (`\`, `;`, etc.)

### ✔️ Soporte de comillas

- Maneja comillas simples: `''`
- Maneja comillas dobles: `""`

### ✔️ Redirecciones

- `<` Redirige la entrada
- `>` Redirige la salida (sobrescribe)
- `>>` Redirige la salida en modo append
- `<<` Here document: lee la entrada hasta encontrar un delimitador (no se guarda en el historial)

### ✔️ Pipes

- Soporte para pipes (`|`) para conectar la salida de un comando con la entrada de otro

### ✔️ Variables de entorno

- Expansión de variables con `$`
- Comandos para manipular el entorno

---

## ⚡ Control de señales

- `ctrl-C`: Muestra un nuevo prompt en una nueva línea
- `ctrl-D`: Sale de la shell
- `ctrl-\`: No hace nada

---

## 🧰 Comandos obligatorios

- `echo` (con opción `-n`)
- `cd` (rutas relativas y absolutas)
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

---

## ⭐ Parte Bonus (No Implementada)

Este proyecto **no incluye** la implementación de la parte bonus, que consiste en:

- ❌ Operadores lógicos `&&` y `||` con paréntesis para prioridades
- ❌ Wildcards `*` para el directorio de trabajo actual

---

## 💬 Notas importantes

- Se permite el uso de `readline()` aunque pueda generar **memory leaks**, pero **tu propio código no debe tener fugas de memoria**.
- El shell no debe interpretar caracteres especiales ni comillas sin cerrar que no sean necesarias según el subject.

---

## 🏗️ Arquitectura del Proyecto

El proyecto sigue una arquitectura modular organizada por funcionalidad:

### 📋 Flujo de Ejecución

1. **Inicialización** → Configuración del entorno y estructuras de datos
2. **Lectura de Input** → Captura de comandos del usuario con readline
3. **Análisis Léxico (Lexer)** → Tokenización del input
4. **Análisis Sintáctico** → Validación de sintaxis
5. **Expansión** → Procesamiento de variables y comillas
6. **Construcción de Comandos** → Creación de la estructura de comandos
7. **Ejecución** → Gestión de pipes, redirecciones y ejecución de comandos

### 📁 Estructura del Proyecto

```
Minishell/
├── Makefile                    # Compilación del proyecto
├── README.md                   # Documentación
├── minishell                   # Ejecutable (generado)
│
├── libft/                      # Librería de funciones auxiliares C
│   ├── ft_*.c                  # Funciones básicas (strings, memory, etc.)
│   ├── get_next_line*.c        # Lectura de archivos línea por línea
│   ├── ft_printf*.c            # Implementación de printf
│   └── libft.h                 # Header de libft
│
├── src/
│   ├── main.c                  # Punto de entrada principal
│   ├── minishell.h             # Header principal con structs y prototipos
│   │
│   ├── init_mini/              # 🚀 Inicialización
│   │   ├── mini_init.c         # Loop principal y gestión de input
│   │   ├── init_struct.c       # Inicialización de estructuras de datos
│   │   └── init_env.c          # Configuración del entorno (variables ENV)
│   │
│   ├── parser/                 # 🔍 Análisis Léxico y Sintáctico
│   │   ├── lexer.c             # Tokenización del input
│   │   ├── lexer_utils.c       # Utilidades del lexer
│   │   ├── handle_quotes.c     # Procesamiento de comillas (' y ")
│   │   ├── handler_args.c      # Manejo de argumentos y palabras
│   │   ├── checker.c           # Validaciones de tokens
│   │   └── syntax/
│   │       ├── syntax_check.c  # Validación de sintaxis
│   │       └── syntax_utils.c  # Utilidades de sintaxis
│   │
│   ├── expander/               # 🔄 Expansión de Variables
│   │   ├── expand_vars.c       # Expansión de variables de entorno ($VAR)
│   │   ├── expand_vars_2.c     # Funciones auxiliares de expansión
│   │   └── expand_vars_3.c     # Manejo de casos especiales ($?, etc.)
│   │
│   ├── concatenate/            # 🔗 Concatenación
│   │   └── concatenate.c       # Unión de tokens y strings
│   │
│   ├── executor/               # ⚙️ Ejecución de Comandos
│   │   ├── executor.c          # Lógica principal de ejecución
│   │   ├── executor_utils.c    # Utilidades de ejecución (fork, wait)
│   │   ├── executor_utils2.c   # Funciones auxiliares adicionales
│   │   └── path_utils.c        # Búsqueda de comandos en PATH
│   │
│   ├── pipes/                  # 🔀 Gestión de Pipes
│   │   ├── pipes.c             # Lógica principal de pipes
│   │   ├── pipes_utils.c       # Creación y gestión de pipes
│   │   ├── pipes_utils2.c      # Ejecución de pipeline
│   │   ├── pipes_utils3.c      # Gestión de file descriptors
│   │   └── pipes_utils4.c      # Utilidades adicionales de pipes
│   │
│   ├── builtins/               # 🛠️ Comandos Built-in
│   │   ├── built.c             # Identificación y ejecución de builtins
│   │   ├── built_utils.c       # Utilidades generales de builtins
│   │   ├── builtin_echo.c      # Comando echo (con -n)
│   │   ├── builtin_cd.c        # Comando cd (cambio de directorio)
│   │   ├── builtin_pwd.c       # Comando pwd (directorio actual)
│   │   ├── builtin_export.c    # Comando export (variables de entorno)
│   │   ├── builtin_unset.c     # Comando unset (eliminar variables)
│   │   ├── builtin_env.c       # Comando env (mostrar entorno)
│   │   └── builtin_exit.c      # Comando exit (salir del shell)
│   │
│   └── utils/                  # 🧰 Utilidades Generales
│       ├── utils.c             # Funciones auxiliares generales
│       ├── utils_2.c           # Más funciones auxiliares
│       ├── free.c              # Liberación de memoria
│       │
│       ├── signals.c           # 📡 Gestión de señales (Ctrl-C, Ctrl-D, Ctrl-\)
│       ├── signals_setup.c     # Configuración de handlers de señales
│       │
│       ├── heredoc.c           # 📄 Here documents (<<)
│       ├── heredoc2.c          # Implementación alternativa/adicional
│       ├── heredoc_utils.c     # Utilidades de heredoc
│       ├── heredoc_utils2.c    # Más utilidades de heredoc
│       │
│       ├── redir_utils.c       # ↔️ Redirecciones (<, >, >>)
│       ├── redir_utils2.c      # Aplicación de redirecciones
│       ├── redir_utils3.c      # Utilidades adicionales
│       │
│       ├── env/                # 🌍 Variables de Entorno
│       │   ├── env_utils.c     # Gestión de lista de entorno
│       │   ├── env_utils2.c    # Conversión y búsqueda
│       │   └── env_utils3.c    # Utilidades adicionales
│       │
│       ├── cmd/                # 📋 Gestión de Comandos
│       │   ├── cmd_utils.c     # Construcción de estructuras de comandos
│       │   └── cmd_utils2.c    # Utilidades adicionales
│       │
│       └── builtin/            # 🔧 Utilidades de Built-ins
│           ├── builtin_cd_utils.c      # Utilidades para cd
│           └── builtin_export_utils.c  # Utilidades para export
│
└── Docs_Tests/                 # 📚 Documentación y Tests
    └── valgrind*               # Configuración y supresiones de valgrind
```

### 🔑 Estructuras de Datos Principales

```c
// Token: Unidad básica del parsing
typedef struct s_token {
    t_token_type    type;       // Tipo: WORD, PIPE, REDIR_IN, etc.
    char            *value;     // Valor del token
    int             has_space;  // Indica si había espacio antes
    struct s_token  *next;      // Lista enlazada
} t_token;

// Comando: Representa un comando a ejecutar
typedef struct s_cmd {
    char            *name;          // Nombre del comando
    char            **args;         // Argumentos
    t_builtin_type  builtin_id;     // ID si es builtin
    t_redir         *redirections;  // Lista de redirecciones
    struct s_cmd    *next;          // Siguiente comando (para pipes)
} t_cmd;

// Redirección: Manejo de <, >, >>, <<
typedef struct s_redir {
    t_token_type    type;       // Tipo de redirección
    char            *file;      // Archivo de entrada/salida
    struct s_redir  *next;      // Lista enlazada
} t_redir;

// Variable de entorno
typedef struct s_env {
    char            *key;       // Nombre de la variable
    char            *value;     // Valor de la variable
    struct s_env    *next;      // Lista enlazada
} t_env;

// Datos principales del shell
typedef struct s_data {
    char        *input;         // Input del usuario
    t_token     *tokens;        // Lista de tokens
    t_cmd       *cmd;           // Lista de comandos
    t_env       *env;           // Variables de entorno
    char        **path;         // Rutas del PATH
    int         exit_status;    // Último código de salida
} t_data;
```

## 💻 Compilación y Uso

```bash
# Compilar el proyecto
make

# Ejecutar minishell
./minishell

# Limpiar archivos objeto
make clean

# Limpiar todo (incluido ejecutable)
make fclean

# Recompilar desde cero
make re
```

### 🔍 Verificación de Memory Leaks

```bash
# Ejecutar con valgrind
valgrind --leak-check=full --show-leak-kinds=all \
         --suppressions=Docs_Tests/valgrind.supp ./minishell
```

---

## 📚 Recursos y Referencias

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/)
- [Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/)
- [Unix Pipes and Filters](https://en.wikipedia.org/wiki/Pipeline_(Unix))

---

## 👥 Autores

- LinkedIn: https://www.linkedin.com/in/miguelfdezmunoz
- LinkedIn: https://www.linkedin.com/in/luisruiztorres

---

## ⚠️ Nota Legal

Este repositorio se comparte con propósito educativo. Está diseñado para servir como:
- 📖 Material de estudio y referencia
- 💡 Guía para comprender la arquitectura de un shell
- 🎯 Ejemplo de buenas prácticas en C

**No está destinado para copia directa.** Se espera que los estudiantes lo utilicen para entender conceptos y desarrollar sus propias implementaciones.

---

<p align="center">🚀 Never stop learning!</p>