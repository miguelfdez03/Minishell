#!/bin/bash

# Script de pruebas para CD

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║             CD TEST SUITE - MINISHELL                  ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}"
echo ""

PASSED=0
FAILED=0
TEST_NUM=1

# Crear directorios de prueba
mkdir -p /tmp/minishell_test_dir
mkdir -p /tmp/minishell_test_no_exec
chmod 000 /tmp/minishell_test_no_exec
touch /tmp/minishell_test_file

test_cd() {
    local cmd="$1"
    local desc="$2"
    local check_cmd="${3:-pwd}"
    
    echo -e "${YELLOW}Test $TEST_NUM: $desc${NC}"
    echo -e "${BLUE}Comando: $cmd${NC}"
    
    # Bash
    echo -e "${GREEN}BASH:${NC}"
    bash_output=$(bash -c "cd /tmp && $cmd && $check_cmd" 2>&1)
    bash_exit=$?
    echo "$bash_output"
    echo "Exit code: $bash_exit"
    
    # Minishell
    echo -e "${GREEN}MINISHELL:${NC}"
    mini_output=$(echo -e "cd /tmp\n$cmd\n$check_cmd\nexit" | ./minishell 2>&1 | grep -v "spidershell>" | grep -v "^exit$" | tail -2 | head -1)
    echo "$mini_output"
    
    # Comparar
    if [ "$bash_exit" -eq 0 ]; then
        if [ -n "$mini_output" ]; then
            echo -e "${GREEN}✅ Ejecutó correctamente${NC}"
            ((PASSED++))
        else
            echo -e "${RED}❌ No produjo output esperado${NC}"
            ((FAILED++))
        fi
    else
        # Bash falló, minishell también debería fallar
        if echo "$mini_output" | grep -q "error\|Error\|cannot\|No such"; then
            echo -e "${GREEN}✅ Error apropiado${NC}"
            ((PASSED++))
        else
            echo -e "${RED}❌ No manejó error correctamente${NC}"
            ((FAILED++))
        fi
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
test_cd "cd /tmp/minishell_test_dir" "Directorio existe"
test_cd "cd /tmp/minishell_no_existe_xyz" "Directorio no existe"
test_cd "cd /tmp/minishell_test_file" "Es un archivo (no directorio)"
test_cd "cd" "cd sin argumentos (debe ir a HOME)"
test_cd "           cd" "cd con espacios"

# Tests especiales
test_cd "cd -" "cd - (OLDPWD)"
test_cd "cd ~" "cd ~ (HOME)"
test_cd "cd /tmp/minishell_test_no_exec" "Directorio sin permisos"

# Cleanup
chmod 755 /tmp/minishell_test_no_exec
rm -rf /tmp/minishell_test_dir /tmp/minishell_test_no_exec /tmp/minishell_test_file

echo -e "${BLUE}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║                   RESUMEN                              ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}"
echo -e "${GREEN}Pasados: $PASSED${NC}"
echo -e "${RED}Fallados: $FAILED${NC}"
echo -e "${YELLOW}Total: $((TEST_NUM - 1))${NC}"
