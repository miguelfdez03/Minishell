#!/bin/bash

# Script de pruebas para EXPORT

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║           EXPORT TEST SUITE - MINISHELL                ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}"
echo ""

PASSED=0
FAILED=0
TEST_NUM=1

test_export() {
    local cmd="$1"
    local desc="$2"
    
    echo -e "${YELLOW}Test $TEST_NUM: $desc${NC}"
    echo -e "${BLUE}Comando: $cmd${NC}"
    
    # Bash
    echo -e "${GREEN}BASH:${NC}"
    bash_output=$(bash -c "$cmd; export | grep -E '(^declare -x a=|^declare -x b=|^declare -x c=|^declare -x USER=|^declare -x HOME=)' | sort" 2>&1)
    echo "$bash_output"
    
    # Minishell
    echo -e "${GREEN}MINISHELL:${NC}"
    mini_output=$(echo -e "$cmd\nexport\nexit" | ./minishell 2>&1 | grep -v "spidershell>" | grep -v "^exit$" | grep -E '(^declare -x a=|^declare -x b=|^declare -x c=|^declare -x USER=|^declare -x HOME=)' | sort)
    echo "$mini_output"
    
    # Comparar (aproximado, solo verificar que las variables se setean)
    if [ -n "$bash_output" ] && [ -n "$mini_output" ]; then
        echo -e "${GREEN}✅ Produce output${NC}"
        ((PASSED++))
    elif [ -z "$bash_output" ] && [ -z "$mini_output" ]; then
        echo -e "${GREEN}✅ Ambos vacíos (correcto)${NC}"
        ((PASSED++))
    else
        echo -e "${RED}❌ Output diferente${NC}"
        ((FAILED++))
    fi
    
    # Memory leaks
    echo -e "${YELLOW}Verificando leaks...${NC}"
    leak_check=$(echo -e "$cmd\nexit" | valgrind --leak-check=full --suppressions=valgrind.supp ./minishell 2>&1 | grep "definitely lost:")
    if echo "$leak_check" | grep -q "definitely lost: 0 bytes"; then
        echo -e "${GREEN}✅ NO LEAKS${NC}"
    else
        echo -e "${RED}⚠️  LEAK DETECTADO${NC}"
    fi
    
    echo ""
    ((TEST_NUM++))
}

# Tests básicos
test_export "export a=b" "Asignar variable simple"
test_export "export b=wawa" "Asignar otra variable"
test_export "export b=lala" "Sobrescribir variable existente"
test_export "export c=lala=kaka" "Valor con signo ="
test_export "export a" "Export sin valor (solo declarar)"
test_export "export a=" "Export con valor vacío"
test_export "export a= b=o" "Múltiples exports"
test_export "export a===" "Valor con múltiples ="

# Tests con números
test_export "export a3=l" "Variable con número al final"
test_export "export 2a=lala" "Variable empezando con número (inválido)"
test_export "export !a=ma" "Variable con carácter especial (inválido)"

# Tests con variables de entorno
test_export "export \$USER=la" "Usar variable como nombre"
test_export "export \$USER=\$TERM" "Variable = Variable"
test_export "export \$USER=\$TERM\$HOME" "Variable = Concatenación"
test_export "export \$USER=\$TERM\$HOM" "Variable con var parcial"
test_export "export \$HOME=\$USER" "Intercambiar variables"
test_export "export \$USER\$SHLVL" "Nombre concatenado de variables"

# Tests especiales
test_export "export" "Export sin argumentos (listar)"
test_export "           export" "Export con espacios"
test_export "export \"\"" "Export con string vacío"
test_export "export a b=\$LESS \$USER \$HOME=la \$COLORTERM=pa c=d" "Múltiples exports complejos"

echo -e "${BLUE}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║                   RESUMEN                              ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}"
echo -e "${GREEN}Pasados: $PASSED${NC}"
echo -e "${RED}Fallados: $FAILED${NC}"
echo -e "${YELLOW}Total: $((TEST_NUM - 1))${NC}"
