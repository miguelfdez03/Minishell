# Checklist de Sintaxis para Minishell (proyecto 42)

Este documento reúne TODO lo que debes tener en cuenta para la parte de comprobación de sintaxis en el proyecto Minishell. Incluye: reglas, casos de fallo, mensajes recomendados, ejemplos y una batería de tests para validar el comportamiento.

Índice
- Requisitos generales
- Reglas de tokenización y prioridad de chequeos
- Lista exhaustiva de errores de sintaxis con ejemplos
- Casos especiales y edge-cases
- Comportamiento con comillas (multi-line continuation)
- Interacciones con expansión de variables y heredoc
- Mensajes de error y normas de salida
- Tests y checklist de validación
- Sugerencias de implementación y orden de chequeo

---

## 1) Requisitos generales

- El parser debe distinguir tokens: WORD, STRING, SIMPLE_Q, PIPE, REDIR_IN (`<`), REDIR_OUT (`>`), REDIR_APPEND (`>>`), HEREDOC (`<<`), ARGS, etc.
- Detectar errores sintácticos antes de ejecutar cualquier comando.
- Comportamiento a imitar: calidad y mensajes similares a `bash` (en particular mensajes `syntax error near unexpected token`).
- Para comillas sin cerrar: NO terminar con error inmediato; pedir al usuario más input (prompt de continuación, p. ej. `> `) y concatenar hasta cerrar la comilla.

---

## 2) Prioridad de comprobaciones (orden recomendado)

1. Verificar comillas abiertas/cerradas (antes de tokenizar completamente): si hay comillas sin cerrar, pedir continuación de línea.
2. Tokenizar (crear lista de tokens).
3. Reglas generales:
   - Línea vacía o solo espacios → no error, continuar.
   - Primer token no puede ser un operador como `|` (error).
4. Recorre tokens y comprobar:
   - Pipes (`|`) (inicio/final/consecutivos)
   - Redirecciones (sin archivo, seguidas de operador, ambigüedad)
   - Heredoc (sintaxis y delimitador)
   - Operadores inválidos o combinaciones no soportadas
5. Después de pasar las comprobaciones: preparar estructura de comando para ejecución.

> Nota: algunas comprobaciones (p. ej. redirecciones sin archivo) deben considerar tokens posteriores que pueden ser STRING, WORD o incluso ARGS.

---

## 3) Errores con `|` (pipes)

Casos a detectar y mensaje recomendado similar a bash:

- Pipe al inicio
  - Input: `| ls`
  - Mensaje: `syntax error near unexpected token '|'`

- Pipe al final
  - Input: `ls |`
  - Mensaje: `syntax error near unexpected token 'newline'`

- Pipes consecutivos
  - Input: `ls || cat` o `ls | | cat`
  - Mensaje: `syntax error near unexpected token '|'`

- Solo pipe (una línea únicamente `|`)
  - Mensaje: `syntax error near unexpected token '|'`

- Múltiples pipes mal formados
  - Input: `ls | | | cat`
  - Mensaje: `syntax error near unexpected token '|'`

---

## 4) Errores con redirecciones (`<`, `>`, `>>`, `<<`)

- Redirección sin argumento (sin archivo o delimitador)
  - Input: `cat >`  OR `echo hello >` or `cat <`
  - Mensaje: `syntax error near unexpected token 'newline'`

- Redirección seguida por operador
  - Input: `cat < | ls`
  - Mensaje: `syntax error near unexpected token '|'`

- Redirección seguida por otra redirección (sin un nombre de fichero válido entre medias)
  - Input: `cat < >` → `syntax error near unexpected token '>'`

- Heredoc (`<<`) sin delimitador o sin cierre correcto
  - Input: `cat <<` → `syntax error near unexpected token 'newline'`
  - Implementación: si `<<` aparece, debe existir un token inmediatamente después con el delimitador (WORD o STRING). Luego, el parser/ejecutor debe leer líneas hasta encontrar el delimitador por separado.

- Ambiguous redirect / espacios y comillas
  - Input: `echo hello > mi archivo.txt` (sin comillas)
  - Dependiendo de tu tokenización, `mi` y `archivo.txt` pueden ser tokens separados, lo que causa ambigüedad; recomendar: exigir comillas o concatenar tokens de nombre si están entre comillas.

- Múltiples redirecciones: definir comportamiento (bash: la última redirección del mismo tipo prevalece)
  - `cmd > out1 > out2` → escribir en `out2`.

---

## 5) Comillas y multiline continuation

### Reglas principales
- Comillas simples (`'`) → no se realiza expansión dentro; todo es literal hasta la comilla `'` de cierre.
- Comillas dobles (`"`) → se realiza expansión ($VAR, $?, $$) dentro; backslash `\` puede escapar ciertos caracteres según implementación mínima.

### Comillas sin cerrar
- Si el input tiene una comilla sin cerrar (simple o doble), no debes fallar con `syntax error` inmediato.
- Comportamiento esperado: mostrar un prompt de continuación (p. ej. `> `), leer más input y concatenar (con un `\n` o no, según convención) hasta que la comilla se cierre.
- Ejemplo:
  ```bash
  spidershell> echo "hola
  > mundo
  > !"
  hola
  mundo
  !
  spidershell>
  ```

### Detalles de implementación
- Detectar comillas no cerradas antes de tokenizar o como resultado de `handle_quotes()` que devuelve un código especial (p. ej. -2) para indicar falta de cierre.
- Mientras haya comilla sin cerrar:
  - readline con prompt de continuación `> `
  - concatenar a la línea original (incluye un `\n` si quieres mantener la separación original)
  - reintentar el parseo/tokenización hasta que cierre

---

## 6) Operadores no soportados y tokens inválidos

- Si implementas solo un subconjunto de operadores, detecta y reporta:
  - `&` (background) → si no soportado, `syntax error near unexpected token '&'` o `command not supported`.
  - `;` (separador de comandos) → si no soportado, `syntax error near unexpected token ';'`.
  - `&&`, `||` → si no soportados, tratarlos como errores sintácticos si aparecen como operadores lógicos.
  - Paréntesis y subshells `(` `)` → comprobar paridad; si no soportados, reportar error.

---

## 7) Heredoc (`<<`) y su interacción con comillas y expansión

- Sintaxis básica: `cmd << DELIM` donde `DELIM` es un token WORD o STRING.
- Lectura del heredoc: el executor debe leer líneas hasta que aparezca una línea exactamente igual a `DELIM`.
- Expansión dentro del heredoc:
  - Si `DELIM` está entre comillas (p. ej. `'EOF'`), NO expandir variables dentro del heredoc.
  - Si no está entre comillas, expandir variables según reglas (como en bash).

Errores a detectar:
- `<<` sin token delimitador → `syntax error near unexpected token 'newline'`.
- Delimitador inválido (p. ej. operador) → `syntax error near unexpected token '<token>'`.

---

## 8) Expansión de variables y su efecto en sintaxis

- La expansión puede cambiar la forma de los tokens; pero la comprobación sintáctica debe hacerse sobre tokens (no sobre la cadena bruta) después de la expansión de tokens relevantes.
- Regla: expandir variables en tokens de tipo `WORD` y `STRING` antes de construir `cmd->args`.
- Edge-cases:
  - `$VAR` que contiene espacios o caracteres especiales: si el valor expandido contiene espacios, se deben mantener dentro del token si originalmente estaba entre comillas; si no, puede provocar nuevos tokens (split) → decidir comportamiento y ser consistente.
  - Variables inexistentes → se reemplazan por cadena vacía.
  - `$?` y `$$` → se expanden antes de la construcción de args y pueden afectar redirecciones si resultan en nombres vacíos.

---

## 9) Mensajes de error y formato (recomendado)

- Usa mensajes similares a `bash` para que los tests sean intuitivos. Ejemplos:
  - `syntax error near unexpected token '|'`
  - `syntax error near unexpected token 'newline'`
  - `syntax error near unexpected token '>'`
  - Para comillas no cerradas: mejor prompt de continuación en vez de error directo.

- Añadir un código de salida al `exit_status` del shell cuando haya error de sintaxis (por ejemplo 2 como hace bash para errores de sintaxis en algunos casos).

---

## 10) Casos especiales y edge-cases (lista extensa)

- `echo "` → pedir continuación.
- `echo '` → pedir continuación.
- `ls | grep` → válido.
- `| ls` → error.
- `ls >` → error.
- `ls > |` → error.
- `cat << EOF` seguido por EOF faltante → si el usuario manda EOF (Ctrl+D) antes de terminar, tratar heredoc como terminado (según la implementación) o error controlado.
- `echo $VAR` donde el valor contiene `|` o `>`: si el token fue sin comillas, podrías interpretar los caracteres resultantes como operadores — para evitar esto, la expansión no debe re-tokenizar (o debes definir reglas explícitas).
- `>file` sin espacio → considerar válido: token `>` seguido de `file` como parte del mismo token si tu tokenizador lo implementa así.
- Backslash `\\` antes de comillas: decide si lo soportas y cómo (mínimo: no requerir tratamiento especial dentro de comillas simples).

---

## 11) Tests y checklist de validación (sugeridos)

Crea un script de tests con casos agrupados por categoría.

Pipes:
```sh
# Deben producir error
| ls
ls |
ls || cat
```

Redirecciones:
```sh
# Error
cat <
echo >
# Válido
echo hello > file
cat < file
```

Comillas:
```sh
# Debe pedir continuación
echo "hola
# Tras introducir: mundo"
# Resultado: hola\nmundo
```

Heredoc:
```sh
cat << EOF
line1
EOF
```

Expansión:
```sh
VAR="a b"
echo $VAR          # comportamiento definido respecto a split
```

Errores combinados:
```sh
| >
< |
```

---

## 12) Sugerencias de implementación (algoritmo y helpers)

- Helpers ya presentes en tu repo a usar: `handle_quotes()`, `handle_words()`, `handle_args()`, `check_redir()`.
- Añadir una función `syntax_check(t_token *tokens)` que reciba lista de tokens y devuelva 0 = ok o -1 = error, y que además imprima el mensaje de error correspondiente.

Pseudocódigo sugerido:
```c
int syntax_check(t_token *tokens)
{
    if (!tokens) return 0; // vacío ok
    if (tokens->type == PIPE) { print_error("|"); return -1; }
    t_token *cur = tokens;
    while (cur)
    {
        if (cur->type == PIPE)
            if (!cur->next) { print_error("newline"); return -1; }
            else if (cur->next->type == PIPE) { print_error("|"); return -1; }
        if (is_redir(cur->type))
        {
            if (!cur->next) { print_error("newline"); return -1; }
            if (is_operator(cur->next->type)) { print_error(token_to_str(cur->next)); return -1; }
        }
        cur = cur->next;
    }
    return 0;
}
```

- Llamar `syntax_check()` justo después de tokenizar y antes de construir `cmd` o ejecutar.
- Detectar comillas sin cerrar en `handle_quotes()` y propagar un código especial al `lexer()` para que el `main_loop()` pida input adicional.

---

## 13) Mensajes recomendados y códigos de salida

- `syntax error near unexpected token 'X'` → código de salida 2 (o el que prefieras para los tests)
- Para comillas sin cerrar → no error, prompt de continuación; si el usuario envía EOF inesperado, imprimir `syntax error: unexpected end of file` y establecer `exit_status` apropiado.

---

## 14) Resumen final

1. Detectar comillas sin cerrar inmediatamente y pedirl continuación.
2. Tokenizar y luego hacer `syntax_check()` en la lista de tokens.
3. En `syntax_check()`, validar pipes y redirecciones (inicio/final/consecutivos/seguidos por operadores).
4. Para heredoc, validar delimitador y coordinar con el executor para leer el contenido.
5. Tratar casos especiales donde la expansión crea ambigüedad con cautela (preferible: no re-tokenizar después de la expansión a no ser que esté entre comillas).
6. Proveer una batería de tests que cubran cada caso enumerado en este documento.

---

Si quieres, puedo:
- Añadir el código base de `syntax_check()` listo para adaptar a vuestra estructura de tokens.
- Generar un script `tests/syntax_tests.sh` con todas las entradas de prueba.
- Implementar el prompt de continuación en `main_loop()` y la lógica de concatenación de líneas para comillas abiertas.

Dime cuál de esas siguientes tareas quieres que haga a continuación.