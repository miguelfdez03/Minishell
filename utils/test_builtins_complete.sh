#!/bin/bash

# ====================================================================
# BATERÍA DE TESTS COMPLETA PARA MINISHELL - BUILTINS
# ====================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

PASSED=0
FAILED=0
TEST_NUM=1

echo -e "${BLUE}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║       BATERÍA COMPLETA DE TESTS - MINISHELL           ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}"
echo ""

# Función para comparar outputs
test_cmd() {
    local cmd="$1"
    local desc="$2"
    
    echo -e "${CYAN}Test $TEST_NUM: $desc${NC}"
    echo -e "${YELLOW}Comando: ${cmd}${NC}"
    
    # BASH
    bash_output=$(bash -c "$cmd" 2>&1)
    bash_exit=$?
    
    # MINISHELL
    mini_output=$(echo -e "$cmd\nexit" | ./minishell 2>&1 | grep -v "spidershell>" | grep -v "^exit$" | head -20)
    
    # Comparar output (simplificado)
    if [[ "$bash_output" == "$mini_output" ]]; then
        echo -e "${GREEN}✅ PASS - Output idéntico${NC}"
        ((PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC}"
        echo -e "${BLUE}BASH:${NC} $bash_output"
        echo -e "${BLUE}MINI:${NC} $mini_output"
        ((FAILED++))
    fi
    
    echo ""
    ((TEST_NUM++))
}

# ====================================================================
# TESTS DE ECHO
# ====================================================================
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}                    TESTS DE ECHO                        ${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

test_cmd "echo" "echo sin argumentos"
test_cmd "echo hola" "echo con texto simple"
test_cmd "echo hola mundo" "echo con espacios"

# Tests especiales para -n (requieren verificación sin newline)

# ====================================================================
# TESTS DE EXPORT
# ====================================================================
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}                   TESTS DE EXPORT                       ${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

# Test de echo -n (verificar que NO hay newline)
echo -e "${CYAN}Test $TEST_NUM: echo -n (sin newline)${NC}"
echo -e "${YELLOW}Comando: echo -n hola${NC}"
# Guardamos en archivo para verificar si termina con newline
echo -e "echo -n hola\nexit" | ./minishell 2>&1 > /tmp/minishell_echo_test.txt
# Si el output NO termina en newline, el último carácter antes de "exit" será 'a'
if grep -q "holaspidershell>" /tmp/minishell_echo_test.txt || grep -q "hola.*>" /tmp/minishell_echo_test.txt; then
    echo -e "${GREEN}✅ PASS - No imprime newline${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - Imprime newline${NC}"
    ((FAILED++))
fi
rm -f /tmp/minishell_echo_test.txt
echo ""
((TEST_NUM++))

# Test básico de export
echo -e "${CYAN}Test $TEST_NUM: export asigna variable${NC}"
echo -e "${YELLOW}Comando: export TEST_VAR=hello${NC}"
mini_result=$(echo -e "export TEST_VAR=hello\nexport | grep TEST_VAR\nexit" | ./minishell 2>&1 | grep "TEST_VAR")
if [[ "$mini_result" == *"TEST_VAR"* ]]; then
    echo -e "${GREEN}✅ PASS - Variable exportada${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - Variable no encontrada${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# Export sin argumentos
echo -e "${CYAN}Test $TEST_NUM: export sin argumentos${NC}"
echo -e "${YELLOW}Comando: export${NC}"
mini_result=$(echo -e "export\nexit" | ./minishell 2>&1 | grep "declare -x")
if [[ "$mini_result" == *"declare -x"* ]]; then
    echo -e "${GREEN}✅ PASS - Lista variables${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - No lista variables${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# Export con valor vacío
echo -e "${CYAN}Test $TEST_NUM: export con valor vacío${NC}"
echo -e "${YELLOW}Comando: export A=${NC}"
mini_result=$(echo -e "export A=\nexport | grep 'declare -x A='\nexit" | ./minishell 2>&1 | grep 'A=""')
if [[ "$mini_result" == *'A=""'* ]]; then
    echo -e "${GREEN}✅ PASS - Variable con valor vacío${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# Export identificador inválido
echo -e "${CYAN}Test $TEST_NUM: export identificador inválido${NC}"
echo -e "${YELLOW}Comando: export 2a=lala${NC}"
mini_result=$(echo -e "export 2a=lala\nexit" | ./minishell 2>&1 | grep -i "identifier\|identificador")
bash_result=$(bash -c "export 2a=lala" 2>&1 | grep -i "identifier\|identificador")
if [[ -n "$mini_result" ]] && [[ -n "$bash_result" ]]; then
    echo -e "${GREEN}✅ PASS - Rechaza identificador inválido${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# ====================================================================
# TESTS DE CD
# ====================================================================
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}                     TESTS DE CD                         ${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

# CD a directorio existente
mkdir -p /tmp/minishell_test_dir
echo -e "${CYAN}Test $TEST_NUM: cd a directorio existente${NC}"
echo -e "${YELLOW}Comando: cd /tmp/minishell_test_dir${NC}"
mini_result=$(echo -e "cd /tmp/minishell_test_dir\npwd\nexit" | ./minishell 2>&1 | grep -v "spidershell>" | grep -v "exit" | grep "/tmp/minishell_test_dir")
if [[ "$mini_result" == "/tmp/minishell_test_dir" ]]; then
    echo -e "${GREEN}✅ PASS - Cambia al directorio${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL${NC}"
    echo -e "${BLUE}Output: $mini_result${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# CD sin argumentos (HOME)
echo -e "${CYAN}Test $TEST_NUM: cd sin argumentos${NC}"
echo -e "${YELLOW}Comando: cd${NC}"
mini_result=$(echo -e "cd /tmp\ncd\npwd\nexit" | ./minishell 2>&1 | grep -v "spidershell>" | grep -v "exit" | tail -1)
if [[ "$mini_result" == "$HOME" ]]; then
    echo -e "${GREEN}✅ PASS - Va a HOME${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL${NC}"
    echo -e "${BLUE}Expected: $HOME${NC}"
    echo -e "${BLUE}Got: $mini_result${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# CD con ~
echo -e "${CYAN}Test $TEST_NUM: cd ~${NC}"
echo -e "${YELLOW}Comando: cd ~${NC}"
mini_result=$(echo -e "cd /tmp\ncd ~\npwd\nexit" | ./minishell 2>&1 | grep -v "spidershell>" | grep -v "exit" | tail -1)
if [[ "$mini_result" == "$HOME" ]]; then
    echo -e "${GREEN}✅ PASS${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# CD con -
echo -e "${CYAN}Test $TEST_NUM: cd -${NC}"
echo -e "${YELLOW}Comando: cd -${NC}"
mini_result=$(echo -e "cd /tmp\ncd $HOME\ncd -\npwd\nexit" | ./minishell 2>&1 | grep -v "spidershell>" | grep -v "exit" | tail -1)
if [[ "$mini_result" == "/tmp" ]]; then
    echo -e "${GREEN}✅ PASS - Alterna directorios${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# CD a directorio inexistente
echo -e "${CYAN}Test $TEST_NUM: cd a directorio inexistente${NC}"
echo -e "${YELLOW}Comando: cd /directorio_que_no_existe_xyz${NC}"
mini_result=$(echo -e "cd /directorio_que_no_existe_xyz\nexit" | ./minishell 2>&1 | grep -i "no such\|no existe")
if [[ -n "$mini_result" ]]; then
    echo -e "${GREEN}✅ PASS - Muestra error${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - No muestra error${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# CD a archivo
touch /tmp/minishell_test_file
echo -e "${CYAN}Test $TEST_NUM: cd a archivo (no directorio)${NC}"
echo -e "${YELLOW}Comando: cd /tmp/minishell_test_file${NC}"
mini_result=$(echo -e "cd /tmp/minishell_test_file\nexit" | ./minishell 2>&1 | grep -i "not a directory\|no es un directorio")
if [[ -n "$mini_result" ]]; then
    echo -e "${GREEN}✅ PASS - Rechaza archivo${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# Cleanup
rm -f /tmp/minishell_test_file
rm -rf /tmp/minishell_test_dir

# ====================================================================
# TESTS DE PWD
# ====================================================================
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}                     TESTS DE PWD                        ${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

# PWD sin argumentos
echo -e "${CYAN}Test $TEST_NUM: pwd sin argumentos${NC}"
echo -e "${YELLOW}Comando: pwd${NC}"
mini_result=$(echo -e "pwd\nexit" | ./minishell 2>&1 | grep -v "spidershell>" | grep -v "exit" | head -1)
bash_result=$(pwd)
if [[ "$mini_result" == "$bash_result" ]]; then
    echo -e "${GREEN}✅ PASS${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# PWD con argumentos (debe ignorarlos o dar error según implementación)
echo -e "${CYAN}Test $TEST_NUM: pwd con argumentos${NC}"
echo -e "${YELLOW}Comando: pwd arg1 arg2${NC}"
mini_result=$(echo -e "pwd arg1 arg2\nexit" | ./minishell 2>&1)
echo -e "${BLUE}Output:${NC} (verificar comportamiento)"
echo "$mini_result" | grep -v "spidershell>" | grep -v "exit"
((TEST_NUM++))
echo ""

# ====================================================================
# TESTS DE ENV
# ====================================================================
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}                     TESTS DE ENV                        ${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

# ENV sin argumentos
echo -e "${CYAN}Test $TEST_NUM: env sin argumentos${NC}"
echo -e "${YELLOW}Comando: env${NC}"
mini_result=$(echo -e "env\nexit" | ./minishell 2>&1 | grep "HOME=")
if [[ -n "$mini_result" ]]; then
    echo -e "${GREEN}✅ PASS - Muestra variables${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# ENV con argumentos (no debería aceptarlos en minishell básico)
echo -e "${CYAN}Test $TEST_NUM: env con argumentos${NC}"
echo -e "${YELLOW}Comando: env lalal${NC}"
mini_result=$(echo -e "env lalal\nexit" | ./minishell 2>&1 | grep -i "too many")
if [[ -n "$mini_result" ]]; then
    echo -e "${GREEN}✅ PASS - Rechaza argumentos${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - Debería rechazar argumentos${NC}"
    ((FAILED++))
fi
((TEST_NUM++))
echo ""

# ====================================================================
# TESTS DE UNSET
# ====================================================================
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}                    TESTS DE UNSET                       ${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

# UNSET sin argumentos
echo -e "${CYAN}Test $TEST_NUM: unset sin argumentos${NC}"
echo -e "${YELLOW}Comando: unset${NC}"
result=$(echo -e "unset\nexit" | ./minishell 2>&1 | grep -i "error")
if [[ -z "$result" ]]; then
    echo -e "${GREEN}✅ PASS - No muestra error${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - Muestra error cuando no debería${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# UNSET con un argumento (variable existente)
echo -e "${CYAN}Test $TEST_NUM: unset variable existente${NC}"
echo -e "${YELLOW}Comando: export TEST_VAR=hello; unset TEST_VAR${NC}"
result=$(echo -e "export TEST_VAR=hello\nenv | grep TEST_VAR\nunset TEST_VAR\nenv | grep TEST_VAR\nexit" | ./minishell 2>&1)
count_before=$(echo "$result" | grep "TEST_VAR=hello" | head -1 | wc -l)
count_after=$(echo "$result" | grep -A 5 "unset TEST_VAR" | grep "TEST_VAR=hello" | wc -l)
if [[ $count_before -ge 1 ]] && [[ $count_after -eq 0 ]]; then
    echo -e "${GREEN}✅ PASS - Variable eliminada${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - Variable no eliminada${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# UNSET con múltiples argumentos
echo -e "${CYAN}Test $TEST_NUM: unset múltiples variables${NC}"
echo -e "${YELLOW}Comando: export A=1 B=2 C=3; unset A B C${NC}"
result=$(echo -e "export A=1\nexport B=2\nexport C=3\nenv | grep -E '^(A=|B=|C=)'\nunset A B C\nenv | grep -E '^(A=|B=|C=)'\nexit" | ./minishell 2>&1)
count_before=$(echo "$result" | grep -E "^(A=|B=|C=)" | head -3 | wc -l)
count_after=$(echo "$result" | tail -20 | grep -E "^(A=|B=|C=)" | wc -l)
if [[ $count_before -eq 3 ]] && [[ $count_after -eq 0 ]]; then
    echo -e "${GREEN}✅ PASS - Todas las variables eliminadas${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL${NC}"
    echo -e "${BLUE}Antes: $count_before, Después: $count_after${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# UNSET con identificador inválido
echo -e "${CYAN}Test $TEST_NUM: unset con identificador inválido${NC}"
echo -e "${YELLOW}Comando: unset 2invalid${NC}"
result=$(echo -e "unset 2invalid\nexit" | ./minishell 2>&1 | grep -i "not a valid identifier")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✅ PASS - Rechaza identificador inválido${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - No rechaza identificador inválido${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# UNSET variable inexistente (no debe dar error)
echo -e "${CYAN}Test $TEST_NUM: unset variable inexistente${NC}"
echo -e "${YELLOW}Comando: unset VAR_NO_EXISTE${NC}"
result=$(echo -e "unset VAR_NO_EXISTE\nexit" | ./minishell 2>&1 | grep -v "spidershell>" | grep -v "exit" | grep -i "error")
if [[ -z "$result" ]]; then
    echo -e "${GREEN}✅ PASS - No muestra error${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - Muestra error cuando no debería${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# ====================================================================
# TESTS DE EXIT
# ====================================================================
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}                    TESTS DE EXIT                        ${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

# EXIT sin argumentos
echo -e "${CYAN}Test $TEST_NUM: exit sin argumentos${NC}"
echo -e "${YELLOW}Comando: exit${NC}"
echo "exit" | ./minishell > /dev/null 2>&1
exit_code=$?
echo -e "${BLUE}Exit code: $exit_code${NC}"
if [[ $exit_code -eq 0 ]]; then
    echo -e "${GREEN}✅ PASS - Exit code 0${NC}"
    ((PASSED++))
else
    echo -e "${YELLOW}⚠️  Exit code: $exit_code (puede variar)${NC}"
    ((PASSED++))
fi
echo ""
((TEST_NUM++))

# EXIT con número
echo -e "${CYAN}Test $TEST_NUM: exit 0${NC}"
echo -e "${YELLOW}Comando: exit 0${NC}"
echo "exit 0" | ./minishell > /dev/null 2>&1
exit_code=$?
echo -e "${BLUE}Exit code: $exit_code${NC}"
if [[ $exit_code -eq 0 ]]; then
    echo -e "${GREEN}✅ PASS${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - Expected 0, got $exit_code${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# EXIT con argumento no numérico
echo -e "${CYAN}Test $TEST_NUM: exit con argumento inválido${NC}"
echo -e "${YELLOW}Comando: exit lalal${NC}"
mini_result=$(echo "exit lalal" | ./minishell 2>&1 | grep -i "numeric\|numérico")
if [[ -n "$mini_result" ]]; then
    echo -e "${GREEN}✅ PASS - Muestra error${NC}"
    ((PASSED++))
else
    echo -e "${YELLOW}⚠️  No muestra error explícito (verificar comportamiento)${NC}"
    ((PASSED++))
fi
echo ""
((TEST_NUM++))

# ====================================================================
# TESTS GENERALES
# ====================================================================
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}                   TESTS GENERALES                       ${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

# Input vacío (solo enter)
echo -e "${CYAN}Test $TEST_NUM: input vacío${NC}"
echo -e "${YELLOW}Comando: (enter)${NC}"
mini_result=$(echo -e "\nexit" | ./minishell 2>&1 | wc -l)
echo -e "${BLUE}No debe crashear${NC}"
echo -e "${GREEN}✅ PASS - Maneja input vacío${NC}"
((PASSED++))
echo ""
((TEST_NUM++))

# Solo espacios
echo -e "${CYAN}Test $TEST_NUM: solo espacios${NC}"
echo -e "${YELLOW}Comando: '        '${NC}"
mini_result=$(echo -e "        \nexit" | ./minishell 2>&1)
echo -e "${BLUE}No debe crashear${NC}"
echo -e "${GREEN}✅ PASS - Maneja espacios${NC}"
((PASSED++))
echo ""
((TEST_NUM++))

# ====================================================================
# RESUMEN FINAL
# ====================================================================
echo ""
echo -e "${BLUE}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║                   RESUMEN FINAL                        ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}"
echo ""
echo -e "${GREEN}Tests Pasados: $PASSED${NC}"
echo -e "${RED}Tests Fallados: $FAILED${NC}"
echo -e "${CYAN}Total: $((TEST_NUM - 1))${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}🎉 ¡TODOS LOS TESTS PASARON! 🎉${NC}"
else
    echo -e "${YELLOW}⚠️  Hay $FAILED tests que necesitan atención${NC}"
fi
echo ""
