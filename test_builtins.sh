#!/bin/bash

# Script de pruebas para ENV, PWD, UNSET, EXIT

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

run_simple_test() {
    local builtin="$1"
    local cmd="$2"
    local desc="$3"
    
    echo -e "${YELLOW}Test: $desc${NC}"
    echo -e "${BLUE}Comando: $cmd${NC}"
    
    # Bash
    echo -e "${GREEN}BASH:${NC}"
    bash -c "$cmd" 2>&1 | head -5
    bash_exit=$?
    echo "Exit code: $bash_exit"
    
    # Minishell
    echo -e "${GREEN}MINISHELL:${NC}"
    echo -e "$cmd\nexit" | ./minishell 2>&1 | grep -v "spidershell>" | grep -v "^exit$" | head -5
    
    # Memory leaks
    echo -e "${YELLOW}Verificando leaks...${NC}"
    leak_check=$(echo -e "$cmd\nexit" | valgrind --leak-check=full --suppressions=valgrind.supp ./minishell 2>&1 | grep "definitely lost:")
    if echo "$leak_check" | grep -q "definitely lost: 0 bytes"; then
        echo -e "${GREEN}✅ NO LEAKS${NC}"
    else
        echo -e "${RED}⚠️  LEAK DETECTADO${NC}"
    fi
    
    echo ""
}

# ============ ENV TESTS ============
echo -e "${BLUE}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║             ENV TEST SUITE                             ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}"
echo ""

run_simple_test "env" "env" "env sin argumentos"
run_simple_test "env" "env lalal" "env con argumento inválido"
run_simple_test "env" "env /tmp" "env con directorio"

# ============ PWD TESTS ============
echo -e "${BLUE}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║             PWD TEST SUITE                             ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}"
echo ""

run_simple_test "pwd" "pwd" "pwd sin argumentos"
run_simple_test "pwd" "pwd lalal" "pwd con argumentos"
run_simple_test "pwd" "pwd /tmp /home" "pwd con múltiples argumentos"

# ============ UNSET TESTS ============
echo -e "${BLUE}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║            UNSET TEST SUITE                            ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}"
echo ""

run_simple_test "unset" "export TEST_VAR=hello && unset TEST_VAR && echo \$TEST_VAR" "unset variable existente"
run_simple_test "unset" "unset" "unset sin argumentos"
run_simple_test "unset" "unset VAR1 VAR2 VAR3" "unset múltiples variables"
run_simple_test "unset" "export A=1 B=2 C=3 && unset A C && export" "unset varias, verificar"

# ============ EXIT TESTS ============
echo -e "${BLUE}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║            EXIT TEST SUITE                             ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}"
echo ""

echo -e "${YELLOW}Test: exit sin argumentos${NC}"
echo "exit" | ./minishell > /dev/null 2>&1
echo "Exit code: $?"

echo -e "${YELLOW}Test: exit 0${NC}"
echo "exit 0" | ./minishell > /dev/null 2>&1
echo "Exit code: $?"

echo -e "${YELLOW}Test: exit 1${NC}"
echo "exit 1" | ./minishell > /dev/null 2>&1
echo "Exit code: $?"

echo -e "${YELLOW}Test: exit 42${NC}"
echo "exit 42" | ./minishell > /dev/null 2>&1
echo "Exit code: $?"

echo -e "${YELLOW}Test: exit lalal (argumento inválido)${NC}"
echo "exit lalal" | ./minishell 2>&1 | grep -v "spidershell>"
echo ""

echo -e "${GREEN}✅ Tests completados${NC}"
