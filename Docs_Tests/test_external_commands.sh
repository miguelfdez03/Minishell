#!/bin/bash

# ====================================================================
# TEST SUITE PARA EXTERNAL COMMANDS
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
echo -e "${BLUE}║        TEST SUITE PARA EXTERNAL COMMANDS              ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}"
echo ""

# Test 1: ls sin argumentos
echo -e "${CYAN}Test $TEST_NUM: ls sin argumentos${NC}"
result=$(echo -e "ls\nexit" | ./minishell 2>&1 | grep -E "Makefile|README")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✅ PASS - ls ejecutado${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - ls no ejecutó${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# Test 2: ls con argumentos
echo -e "${CYAN}Test $TEST_NUM: ls -l${NC}"
result=$(echo -e "ls -l\nexit" | ./minishell 2>&1 | grep -E "total|rwx")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✅ PASS - ls -l ejecutado${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - ls -l no ejecutó${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# Test 3: cat con archivo
echo -e "${CYAN}Test $TEST_NUM: cat README.md${NC}"
result=$(echo -e "cat README.md\nexit" | ./minishell 2>&1 | grep -i "minishell")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✅ PASS - cat ejecutado${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - cat no ejecutó${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# Test 4: grep con múltiples argumentos
echo -e "${CYAN}Test $TEST_NUM: grep NAME Makefile${NC}"
result=$(echo -e "grep NAME Makefile\nexit" | ./minishell 2>&1 | grep "NAME")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✅ PASS - grep ejecutado${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - grep no ejecutó${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# Test 5: wc con archivo
echo -e "${CYAN}Test $TEST_NUM: wc -l Makefile${NC}"
result=$(echo -e "wc -l Makefile\nexit" | ./minishell 2>&1 | grep "Makefile")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✅ PASS - wc ejecutado${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - wc no ejecutó${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# Test 6: /bin/echo (ruta absoluta)
echo -e "${CYAN}Test $TEST_NUM: /bin/echo con ruta absoluta${NC}"
result=$(echo -e "/bin/echo hola\nexit" | ./minishell 2>&1 | grep "hola")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✅ PASS - Ruta absoluta ejecutada${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - Ruta absoluta no ejecutó${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# Test 7: comando que no existe
echo -e "${CYAN}Test $TEST_NUM: comando inexistente${NC}"
result=$(echo -e "comando_que_no_existe\nexit" | ./minishell 2>&1 | grep -i "command not found")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✅ PASS - Error mostrado correctamente${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - No muestra error${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# Test 8: date
echo -e "${CYAN}Test $TEST_NUM: date${NC}"
result=$(echo -e "date\nexit" | ./minishell 2>&1 | grep -E "[0-9]{4}")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✅ PASS - date ejecutado${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - date no ejecutó${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# Test 9: whoami
echo -e "${CYAN}Test $TEST_NUM: whoami${NC}"
current_user=$(whoami)
result=$(echo -e "whoami\nexit" | ./minishell 2>&1 | grep "$current_user")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✅ PASS - whoami ejecutado${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - whoami no ejecutó${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# Test 10: which bash
echo -e "${CYAN}Test $TEST_NUM: which bash${NC}"
result=$(echo -e "which bash\nexit" | ./minishell 2>&1 | grep "/bash")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✅ PASS - which ejecutado${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAIL - which no ejecutó${NC}"
    ((FAILED++))
fi
echo ""
((TEST_NUM++))

# ====================================================================
# RESUMEN
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
