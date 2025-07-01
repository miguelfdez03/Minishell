# 🚀 Minishell

![Minishell Banner](https://github.com/user-attachments/assets/1c069ab2-94ac-47b5-8974-b099c75efa95)

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

## 💬 Notas importantes

- Se permite el uso de `readline()` aunque pueda generar **memory leaks**, pero **tu propio código no debe tener fugas de memoria**.
- El shell no debe interpretar caracteres especiales ni comillas sin cerrar que no sean necesarias según el subject.

---

## 💻 Compilación

```bash
make
1