# Expansión de Variables en Minishell

## Descripción General

El sistema de expansión de variables en Minishell se encarga de reemplazar las referencias a variables de entorno (como `$HOME`, `$USER`) por sus valores reales antes de ejecutar los comandos.

Este sistema está dividido en **3 archivos** que trabajan juntos:

1. **`expand_vars.c`** - Funciones auxiliares básicas
2. **`expand_vars_2.c`** - Lógica principal de expansión
3. **`expand_vars_3.c`** - Expansiones especiales ($? y $$)

---

## Archivo 1: `expand_vars.c`

### Funciones auxiliares fundamentales

#### 1. `is_valid_var_char(char c, int is_first)`

**Propósito:** Determinar si un carácter es válido en un nombre de variable.

```c
is_valid_var_char('A', 1)  → true  (primera letra)
is_valid_var_char('_', 1)  → true  (guión bajo al inicio)
is_valid_var_char('1', 1)  → false (número al inicio)
is_valid_var_char('5', 0)  → true  (número en medio/final)
```

**Reglas:**
- **Primer carácter:** Solo letras (a-z, A-Z) o guión bajo (_)
- **Caracteres siguientes:** Letras, números (0-9) o guión bajo (_)

---

#### 2. `get_var_value(char *var_name, t_env *env)`

**Propósito:** Buscar el valor de una variable en la lista de entorno.

```c
get_var_value("HOME", env)  → "/home/usuario"
get_var_value("USER", env)  → "lruiz-to"
get_var_value("NOEXISTE", env)  → NULL
```

**Funcionamiento:**
1. Recorre la lista enlazada de variables de entorno
2. Compara el nombre de la variable con cada `key`
3. Devuelve el `value` si encuentra coincidencia
4. Devuelve `NULL` si no existe

---

#### 3. `extract_var_name(char *str, int start, int *len)`

**Propósito:** Extraer el nombre de una variable desde el string.

```c
str = "echo $HOME/docs"
extract_var_name(str, 6, &len)  → "HOME" (len = 4)

str = "ls $USER_NAME"
extract_var_name(str, 4, &len)  → "USER_NAME" (len = 9)
```

**Proceso:**
1. Verifica que el primer carácter sea válido
2. Cuenta cuántos caracteres válidos hay consecutivamente
3. Extrae el substring con `ft_substr()`
4. Guarda la longitud en el parámetro `len`

---

#### 4. `append_to_result(char *result, char *to_add)`

**Propósito:** Concatenar strings liberando la memoria anterior.

```c
result = "echo "
append_to_result(result, "hola")  → "echo hola"
// ⚠️ El string original "echo " se libera automáticamente
```

**Casos especiales:**
- Si `to_add` es NULL → devuelve `result` sin cambios
- Si `result` es NULL → devuelve una copia de `to_add`

---

#### 5. `append_char(char *result, char c)`

**Propósito:** Añadir un solo carácter al resultado.

```c
result = "echo"
append_char(result, '!')  → "echo!"
```

**Implementación:**
1. Convierte el char en un string de 2 caracteres: `[c, '\0']`
2. Llama a `append_to_result()` para concatenar

---

## Archivo 2: `expand_vars_2.c`

### Funciones de expansión principales

#### 1. `expand_braced_var(char *str, int *i, t_env *env, char *result)`

**Propósito:** Expandir variables con llaves: `${VARIABLE}`

```c
str = "echo ${HOME}/docs"
         ^
         i = 5

→ Expande a: "/home/usuario"
→ Avanza i hasta después del '}'
```

**Proceso:**
1. Busca el `}` de cierre
2. Extrae el nombre entre `${` y `}`
3. Busca el valor con `get_var_value()`
4. Concatena el valor al resultado
5. Actualiza el índice `i`

**Ventaja de las llaves:**
```bash
${HOME}file  → "/home/usuariofile"  ✅
$HOMEfile    → busca variable "HOMEfile"  ❌
```

---

#### 2. `expand_simple_var(char *str, int *i, t_env *env, char *result)`

**Propósito:** Expandir variables simples: `$VARIABLE`

```c
str = "echo $USER"
         ^
         i = 5

→ Expande a: "lruiz-to"
```

**Proceso:**
1. Extrae el nombre de la variable con `extract_var_name()`
2. Busca su valor en el entorno
3. Si no existe, no añade nada (cadena vacía)
4. Actualiza el índice `i`

**Casos especiales:**
```c
"$"      → añade '$' literal (no hay nombre válido)
"$123"   → añade '$' literal (empieza con número)
"$_VAR"  → expande "_VAR" (guión bajo es válido)
```

---

#### 3. `handle_dollar(char *str, int *i, t_env *env, int exit_status)`

**Propósito:** Decidir qué tipo de expansión hacer según lo que sigue al `$`.

```c
"$HOME"     → expand_simple_var()
"${HOME}"   → expand_braced_var()
"$?"        → expand_exit_status()
"$$"        → expand_pid()
"$ "        → '$' literal
```

**Flujo de decisión:**
```
$? → Exit status del último comando
$$ → Process ID del shell
${ → Variable con llaves
$X → Variable simple (X = letra o _)
$  → $ literal
```

---

#### 4. `expand_string(char *str, t_env *env, int exit_status)`

**Propósito:** Expandir todas las variables en un string completo.

```c
str = "echo $USER is in $HOME"

Proceso:
1. "echo "           → copia literal
2. "$USER"           → expande a "lruiz-to"
3. " is in "         → copia literal
4. "$HOME"           → expande a "/home/lruiz-to"

Resultado: "echo lruiz-to is in /home/lruiz-to"
```

**Algoritmo:**
```
Para cada carácter:
  Si es '$':
    → Llamar handle_dollar() para expandir
    → Añadir el resultado
  Si no:
    → Añadir el carácter literal
```

---

#### 5. `expand_variables(t_token *tokens, t_env *env, int exit_status)`

**Propósito:** Punto de entrada principal. Expande variables en todos los tokens.

```c
Tokens antes:
  [WORD: "echo"]
  [WORD: "$USER"]
  [STRING: "is in $HOME"]

Tokens después:
  [WORD: "echo"]
  [WORD: "lruiz-to"]
  [STRING: "is in /home/lruiz-to"]
```

**Proceso:**
1. Recorre la lista de tokens
2. Solo procesa tokens de tipo `WORD` o `STRING`
3. Expande el valor con `expand_string()`
4. Reemplaza el valor antiguo con el expandido
5. Libera la memoria del valor anterior

---

## Archivo 3: `expand_vars_3.c`

### Expansiones especiales

#### 1. `expand_exit_status(char *result, int exit_status, int *i)`

**Propósito:** Expandir `$?` al código de salida del último comando.

```c
// Después de: ls archivo_inexistente
"echo $?"  → "echo 1"

// Después de: ls /home
"echo $?"  → "echo 0"
```

**Valores comunes:**
- `0` → Éxito
- `1` → Error general
- `127` → Comando no encontrado
- `130` → Terminado con Ctrl+C

**Implementación:**
1. Convierte el número a string con `ft_itoa()`
2. Concatena al resultado
3. Libera la memoria temporal
4. Avanza el índice 2 posiciones (`$?`)

---

#### 2. `expand_pid(char *result, int *i)`

**Propósito:** Expandir `$$` al Process ID del shell actual.

```c
"echo $$"  → "echo 12345"
```

**Uso común:**
```bash
# Crear archivos temporales únicos
touch /tmp/myfile.$$
→ /tmp/myfile.12345
```

**Implementación:**
1. Obtiene el PID con `getpid()`
2. Convierte a string con `ft_itoa()`
3. Concatena al resultado
4. Avanza el índice 2 posiciones (`$$`)

---

## Flujo completo de expansión

### Ejemplo paso a paso

**Input:** `echo $USER is in $HOME with status $?`

```
┌─────────────────────────────────────────────┐
│ 1. TOKENIZACIÓN (antes de expansión)       │
└─────────────────────────────────────────────┘
  Token 1: [WORD] "echo"
  Token 2: [WORD] "$USER"
  Token 3: [WORD] "is"
  Token 4: [WORD] "in"
  Token 5: [WORD] "$HOME"
  Token 6: [WORD] "with"
  Token 7: [WORD] "status"
  Token 8: [WORD] "$?"

┌─────────────────────────────────────────────┐
│ 2. LLAMADA A expand_variables()             │
└─────────────────────────────────────────────┘
  → Recorre cada token
  → Para cada token tipo WORD/STRING:
    → Llama expand_string()

┌─────────────────────────────────────────────┐
│ 3. EXPANSIÓN DE Token 2: "$USER"           │
└─────────────────────────────────────────────┘
  expand_string("$USER", env, 0)
    → Encuentra '$' en posición 0
    → handle_dollar()
      → Detecta variable simple
      → expand_simple_var()
        → extract_var_name() → "USER"
        → get_var_value() → "lruiz-to"
  Resultado: "lruiz-to"

┌─────────────────────────────────────────────┐
│ 4. EXPANSIÓN DE Token 5: "$HOME"           │
└─────────────────────────────────────────────┘
  expand_string("$HOME", env, 0)
    → Encuentra '$'
    → handle_dollar()
      → expand_simple_var()
        → get_var_value() → "/home/lruiz-to"
  Resultado: "/home/lruiz-to"

┌─────────────────────────────────────────────┐
│ 5. EXPANSIÓN DE Token 8: "$?"              │
└─────────────────────────────────────────────┘
  expand_string("$?", env, 0)
    → Encuentra '$'
    → handle_dollar()
      → Detecta '?' después del '$'
      → expand_exit_status()
        → ft_itoa(0) → "0"
  Resultado: "0"

┌─────────────────────────────────────────────┐
│ 6. RESULTADO FINAL (después de expansión)  │
└─────────────────────────────────────────────┘
  Token 1: [WORD] "echo"
  Token 2: [WORD] "lruiz-to"
  Token 3: [WORD] "is"
  Token 4: [WORD] "in"
  Token 5: [WORD] "/home/lruiz-to"
  Token 6: [WORD] "with"
  Token 7: [WORD] "status"
  Token 8: [WORD] "0"
```

---

## Casos especiales y edge cases

### 1. Variables que no existen

```bash
echo $NOEXISTE hola
→ "echo  hola"  # La variable se reemplaza por cadena vacía
```

### 2. $ literal sin variable válida

```bash
echo $123
→ "echo $123"  # No es un nombre válido, se mantiene literal

echo $
→ "echo $"  # No hay nada después, se mantiene literal
```

### 3. Variables con llaves

```bash
echo ${HOME}file
→ "echo /home/usuariofile"  # Las llaves delimitan claramente

echo $HOMEfile
→ "echo "  # Busca la variable "HOMEfile" que no existe
```

### 4. Comillas simples vs dobles

```bash
echo "$HOME"
→ "echo /home/usuario"  # Se expande en comillas dobles

echo '$HOME'
→ "echo $HOME"  # NO se expande en comillas simples
```

**Nota:** La expansión solo se aplica a tokens de tipo `WORD` y `STRING`. Los tokens creados desde comillas simples no se expanden (esto se maneja en el lexer).

### 5. Múltiples variables en un token

```bash
echo $USER:$HOME
→ "echo lruiz-to:/home/lruiz-to"  # Todas se expanden
```

---

## Diagrama de arquitectura

```
┌───────────────────────────────────────────────────────────┐
│                    expand_variables()                     │
│            (Punto de entrada principal)                   │
└────────────────────────┬──────────────────────────────────┘
                         │
                         ▼
            ┌────────────────────────┐
            │   expand_string()      │
            │  (Procesa un string)   │
            └────────┬───────────────┘
                     │
                     ▼
            ┌────────────────────────┐
            │   handle_dollar()      │
            │  (Decide tipo de $)    │
            └────┬───────────────────┘
                 │
       ┌─────────┼──────────┬────────────┐
       ▼         ▼          ▼            ▼
┌──────────┐ ┌────────┐ ┌──────┐  ┌──────────┐
│expand_   │ │expand_ │ │expand│  │expand_   │
│simple_   │ │braced_ │ │_exit_│  │pid()     │
│var()     │ │var()   │ │status│  │          │
└────┬─────┘ └───┬────┘ └──────┘  └──────────┘
     │           │
     │           │
     ▼           ▼
┌─────────────────────────┐
│  extract_var_name()     │
└────────┬────────────────┘
         │
         ▼
┌─────────────────────────┐
│   get_var_value()       │
└─────────────────────────┘
```

---

## Gestión de memoria

### Principio fundamental

**Cada función que modifica `result` debe:**
1. Crear un nuevo string con la concatenación
2. Liberar el `result` anterior
3. Devolver el nuevo string

### Ejemplo de cadena de liberaciones

```c
result = NULL;                    // 1. Inicio

result = append_char(result, 'e'); // 2. result = "e"
// Internamente: free(NULL) → OK

result = append_char(result, 'c'); // 3. result = "ec"
// Internamente: free("e")

result = append_to_result(result, "ho"); // 4. result = "echo"
// Internamente: free("ec")
```

### ⚠️ Importante

```c
// ❌ MAL - Memory leak
char *expanded = expand_string(token->value, env, 0);
// No liberamos token->value antes de sobrescribirlo

// ✅ BIEN - Liberación correcta
char *expanded = expand_string(token->value, env, 0);
if (expanded)
{
    free(token->value);      // Liberar el valor anterior
    token->value = expanded; // Asignar el nuevo valor
}
```

---

## Testing y debugging

### Comandos de prueba

```bash
# Variables básicas
echo $HOME
echo $USER
echo $PATH

# Variables especiales
echo $?
echo $$

# Variables con llaves
echo ${HOME}
echo ${USER}file

# Variables inexistentes
echo $NOEXISTE

# Múltiples variables
echo $USER is in $HOME

# Con comillas
echo "$HOME"
echo '$HOME'

# Combinaciones
echo $USER:$HOME:$?:$$
```

### Añadir debug prints

En `expand_variables()`:

```c
ft_printf("--- ANTES DE EXPANSIÓN ---\n");
print_tokens(tokens);

expand_variables(tokens, env, exit_status);

ft_printf("--- DESPUÉS DE EXPANSIÓN ---\n");
print_tokens(tokens);
```

---

## Resumen de funciones por archivo

| Archivo | Funciones | Responsabilidad |
|---------|-----------|-----------------|
| `expand_vars.c` | 5 funciones | Utilidades básicas (validación, búsqueda, concatenación) |
| `expand_vars_2.c` | 5 funciones | Lógica principal de expansión |
| `expand_vars_3.c` | 2 funciones | Expansiones especiales ($? y $$) |

**Total:** 12 funciones trabajando en conjunto para una expansión completa y robusta.
