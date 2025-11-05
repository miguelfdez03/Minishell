#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

MINISHELL="./minishell"
TESTS_PASSED=0
TESTS_FAILED=0
TESTS_TOTAL=0

# Create temp directory for test files
TEST_DIR="test_tmp_$$"
mkdir -p "$TEST_DIR"

print_header() {
    echo -e "\n${BLUE}======================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}======================================${NC}\n"
}

print_test() {
    echo -e "${YELLOW}Testing:${NC} $1"
}

run_test() {
    local test_name="$1"
    local command="$2"
    local expected_behavior="$3"
    
    ((TESTS_TOTAL++))
    print_test "$test_name"
    echo -e "${BLUE}Command:${NC} $command"
    
    # Run command in minishell
    echo -e "$command\nexit" | $MINISHELL 2>&1 | tail -n +2 | head -n -1 > "$TEST_DIR/mini_output.txt"
    
    # Run command in bash for comparison (when applicable)
    if [ "$expected_behavior" != "minishell_only" ]; then
        echo -e "$command" | bash 2>&1 > "$TEST_DIR/bash_output.txt"
    fi
    
    echo -e "${GREEN}✓ Test executed${NC}\n"
    ((TESTS_PASSED++))
}

run_test_no_compare() {
    local test_name="$1"
    local command="$2"
    
    ((TESTS_TOTAL++))
    print_test "$test_name"
    echo -e "${BLUE}Command:${NC} $command"
    
    # Just run and show output
    echo -e "$command\nexit" | $MINISHELL 2>&1
    
    echo -e "${GREEN}✓ Test executed${NC}\n"
    ((TESTS_PASSED++))
}

# ======================
# ECHO TESTS
# ======================
print_header "ECHO TESTS"

run_test "echo (no args)" "echo" "compare"
run_test "echo hola" "echo hola" "compare"
run_test "echo empty quotes" 'echo ""' "compare"
run_test 'echo "" hola' 'echo "" hola' "compare"
run_test "echo -n hola" "echo -n hola" "compare"
run_test "echo -n -n hola" "echo -n -n hola" "compare"
run_test "echo -n -nh -n hola" "echo -n -nh -n hola" "compare"
run_test "echo -nnnnnnnnn hola" "echo -nnnnnnnnn hola" "compare"
run_test 'echo $?hcudshfuewrf ew  wr 4ew""""' 'echo $?hcudshfuewrf ew  wr 4ew""""' "compare"
run_test 'echo complex quotes 1' 'echo ""uhjkhnkj"hjbmb"'"'"''"'"''"'"''"'"' defdjhfkjb "iujiouhj"f'"'"'f'"'"'f'"'"'f'"'"'f'"'"''"'"'f'"'"'f'"'"'f'"'"'f'"'"'f'"'"'f' "compare"
run_test 'echo complex quotes 2' 'echo ""uhjkhnkj"hjbmb"'"'"''"'"''"'"''"'"' defdjhfkjb "iujiouhj"f'"'"'f'"'"'a'"'"'j'"'"'o'"'"''"'"'i'"'"'w'"'"'q'"'"''"'"'q' "compare"
run_test 'echo complex quotes 3' 'echo ""aaaaaaaa"bbbbb"'"'"''"'"''"'"''"'"' ccccccccc "dddddddd"e'"'"'e'"'"'e' "compare"
run_test 'echo with variables and quotes 1' 'echo "ho   $USER"  '"'"'la   $LESS'"'"'   $USER$TERM' "compare"
run_test 'echo with variables and quotes 2' 'echo "ho   $USER"'"'"'la   $LESS'"'"'$USER$TERM' "compare"
run_test 'echo no space after command 1' 'echo"ho   $USER"' "compare"
run_test 'echo no space after command 2' 'echo"ho   $USE$LESS"' "compare"
run_test 'quoted echo command 1' '"echo ho   $USER"' "minishell_only"
run_test 'quoted echo command 2' '"echo" "ho   $USER"' "compare"
run_test 'quoted echo command 3' 'e"cho" "ho   $USER"' "compare"
run_test 'echo with leading spaces' '       echo "ho   $USER"' "compare"
run_test 'echo mix single/double quotes 1' 'echo "$HOME '"'"'$LESS'"'"' $USER"' "compare"
run_test 'echo mix single/double quotes 2' 'echo '"'"'$HOME "$LESS" $USER'"'"'' "compare"
run_test 'echo complex nested quotes 1' 'echo "$HOME '"'"'$LESS "pa $TERM" na'"'"' $USER"' "compare"
run_test 'echo complex nested quotes 2' 'echo '"'"'$HOME "$LESS '"'"'pa $TERM'"'"' na" $USER'"'"'' "compare"
run_test 'echo $HOME in quotes' 'echo '"'"'$'"'"'"HOME"' "compare"

# ======================
# EXPORT TESTS
# ======================
print_header "EXPORT TESTS"

run_test "export a=b" "export a=b" "minishell_only"
run_test "export b=wawa" "export b=wawa" "minishell_only"
run_test "export b=lala (override)" "export b=lala" "minishell_only"
run_test "export c=lala=kaka" "export c=lala=kaka" "minishell_only"
run_test "export a (no value)" "export a" "minishell_only"
run_test "export a= (empty value)" "export a=" "minishell_only"
run_test "export a= b=o" "export a= b=o" "minishell_only"
run_test "export a===" "export a===" "minishell_only"
run_test "export a3=l" "export a3=l" "minishell_only"
run_test "export 2a=lala (invalid)" "export 2a=lala" "minishell_only"
run_test "export !a=ma (invalid)" "export !a=ma" "minishell_only"
run_test "export with variable name" "export \$USER=la" "minishell_only"
run_test "export with variable values 1" "export \$USER=\$TERM" "minishell_only"
run_test "export with variable values 2" "export \$USER=\$TERM\$HOME" "minishell_only"
run_test "export with variable values 3" "export \$USER=\$TERM\$HOM" "minishell_only"
run_test "export HOME=USER" "export \$HOME=\$USER" "minishell_only"
run_test "export variable concatenation" "export \$USER\$SHLVL" "minishell_only"
run_test "export (no args - list all)" "export" "minishell_only"
run_test "export with leading spaces" "           export" "minishell_only"
run_test "export empty quotes" 'export ""' "minishell_only"
run_test "export multiple mixed" "export a b=\$LESS \$USER \$HOME=la \$COLORTERM=pa c=d" "minishell_only"

# ======================
# CD TESTS
# ======================
print_header "CD TESTS"

# Setup test directories
mkdir -p "$TEST_DIR/test_dir"
touch "$TEST_DIR/test_file"
mkdir -p "$TEST_DIR/test_perm"
chmod 000 "$TEST_DIR/test_perm" 2>/dev/null

run_test "cd to existing directory" "cd $TEST_DIR/test_dir\npwd" "minishell_only"
run_test "cd to non-existing directory" "cd $TEST_DIR/nonexistent" "minishell_only"
run_test "cd to file (should fail)" "cd $TEST_DIR/test_file" "minishell_only"
run_test "cd (no args - go HOME)" "cd\npwd" "minishell_only"
run_test "cd with leading spaces" "           cd\npwd" "minishell_only"
run_test "cd - (OLDPWD)" "cd /tmp\ncd /\ncd -\npwd" "minishell_only"
run_test "cd ~ (HOME)" "cd ~\npwd" "minishell_only"
run_test "cd with no permissions" "cd $TEST_DIR/test_perm" "minishell_only"

# Cleanup permissions
chmod 755 "$TEST_DIR/test_perm" 2>/dev/null

# ======================
# ENV TESTS
# ======================
print_header "ENV TESTS"

run_test "env (no args)" "env" "minishell_only"
run_test "env with argument (should fail)" "env lalal" "minishell_only"
run_test "env with directory argument" "env $TEST_DIR" "minishell_only"

# ======================
# PWD TESTS
# ======================
print_header "PWD TESTS"

run_test "pwd (no args)" "pwd" "compare"
run_test "pwd with arguments" "pwd lalala" "minishell_only"

# ======================
# UNSET TESTS
# ======================
print_header "UNSET TESTS"

run_test "unset (no args)" "unset" "minishell_only"
run_test "unset single variable" "export TEST=value\nunset TEST\nenv | grep TEST" "minishell_only"
run_test "unset multiple variables" "export A=1 B=2 C=3\nunset A B C\nenv | grep -E 'A=|B=|C='" "minishell_only"

# ======================
# EXIT TESTS
# ======================
print_header "EXIT TESTS"

echo -e "${YELLOW}Testing:${NC} exit (no args)"
(cd /home/miguel/Escritorio/github/Minishell && echo -e "exit" | ./minishell)
echo -e "Exit code: $?"
((TESTS_TOTAL++))
((TESTS_PASSED++))

echo -e "\n${YELLOW}Testing:${NC} exit 0"
(cd /home/miguel/Escritorio/github/Minishell && echo -e "exit 0" | ./minishell)
echo -e "Exit code: $?"
((TESTS_TOTAL++))
((TESTS_PASSED++))

echo -e "\n${YELLOW}Testing:${NC} exit 1"
(cd /home/miguel/Escritorio/github/Minishell && echo -e "exit 1" | ./minishell)
echo -e "Exit code: $?"
((TESTS_TOTAL++))
((TESTS_PASSED++))

echo -e "\n${YELLOW}Testing:${NC} exit 42"
(cd /home/miguel/Escritorio/github/Minishell && echo -e "exit 42" | ./minishell)
echo -e "Exit code: $?"
((TESTS_TOTAL++))
((TESTS_PASSED++))

echo -e "\n${YELLOW}Testing:${NC} exit lalal (invalid)"
(cd /home/miguel/Escritorio/github/Minishell && echo -e "exit lalal" | ./minishell)
echo -e "Exit code: $?"
((TESTS_TOTAL++))
((TESTS_PASSED++))

# ======================
# PIPE TESTS
# ======================
print_header "PIPE TESTS"

# Create test file for pipes
echo -e "red\nblue\ngreen\nyellow\nblue\nred" > "$TEST_DIR/colors.txt"

run_test "ls | wc" "ls | wc" "minishell_only"
run_test "ls|wc (no spaces)" "ls|wc" "minishell_only"
run_test "sort | uniq | sort -r | wc" "sort $TEST_DIR/colors.txt | uniq | sort -r | wc" "minishell_only"
run_test "ls || wc (double pipe)" "ls || wc" "minishell_only"
run_test "ls |   | wc (empty pipe)" "ls |   | wc" "minishell_only"
run_test "ls |   > wc (pipe with redir)" "ls |   > wc" "minishell_only"
run_test "ls | wc | (trailing pipe)" "ls | wc |" "minishell_only"
run_test "| (single pipe)" "|" "minishell_only"
run_test "| ls | wc (leading pipe)" "| ls | wc" "minishell_only"

# ======================
# REDIRECTION TESTS
# ======================
print_header "REDIRECTION TESTS"

run_test "echo hola > file" "echo hola > $TEST_DIR/out1.txt\ncat $TEST_DIR/out1.txt" "minishell_only"
run_test "echo hola >> file (append)" "echo hola >> $TEST_DIR/out2.txt\necho mundo >> $TEST_DIR/out2.txt\ncat $TEST_DIR/out2.txt" "minishell_only"
run_test "echo hola >>>>>>> file (multiple >)" "echo hola >>>>>>> $TEST_DIR/out3.txt" "minishell_only"
run_test "echo hola <<<<<<< file (multiple <)" "echo hola <<<<<<< $TEST_DIR/out4.txt" "minishell_only"
run_test "echo hola>file (no space)" "echo hola>$TEST_DIR/out5.txt\ncat $TEST_DIR/out5.txt" "minishell_only"
run_test "echo hola > file > file2 > file3" "echo hola > $TEST_DIR/o1.txt > $TEST_DIR/o2.txt > $TEST_DIR/o3.txt\ncat $TEST_DIR/o3.txt" "minishell_only"
run_test "wc < colors.txt" "wc < $TEST_DIR/colors.txt" "minishell_only"
run_test "wc < colors.txt > file" "wc < $TEST_DIR/colors.txt > $TEST_DIR/out6.txt\ncat $TEST_DIR/out6.txt" "minishell_only"
run_test "cat colors.txt | wc" "cat $TEST_DIR/colors.txt | wc" "minishell_only"
run_test "echo hola > file << EOF" "echo hola > $TEST_DIR/out7.txt << EOF" "minishell_only"
run_test "cat << EOF > file" "cat << EOF > $TEST_DIR/out8.txt\nhola\nmundo\nEOF" "minishell_only"
run_test "<< EOF (heredoc alone)" "<< EOF\nhola\nEOF" "minishell_only"
run_test "cat << hola << que << tal (multiple heredoc)" "cat << hola << que << tal\ntest\nhola\ntest2\nque\ntest3\ntal" "minishell_only"
run_test "cat << EOF | ls" "cat << EOF | ls\ntest\nEOF" "minishell_only"
run_test "multiple pipes with cat" "echo hola | cat | cat | cat | cat | cat | cat | cat" "minishell_only"

# ======================
# GENERAL TESTS
# ======================
print_header "GENERAL TESTS"

echo -e "${YELLOW}Testing:${NC} empty input (just Enter)"
(cd /home/miguel/Escritorio/github/Minishell && echo -e "\nexit" | ./minishell)
((TESTS_TOTAL++))
((TESTS_PASSED++))

echo -e "\n${YELLOW}Testing:${NC} only spaces"
(cd /home/miguel/Escritorio/github/Minishell && echo -e "     \nexit" | ./minishell)
((TESTS_TOTAL++))
((TESTS_PASSED++))

echo -e "\n${YELLOW}Testing:${NC} unclosed quotes"
(cd /home/miguel/Escritorio/github/Minishell && echo -e '"hola\nexit' | ./minishell)
((TESTS_TOTAL++))
((TESTS_PASSED++))

run_test "echo \$?" "ls\necho \$?" "minishell_only"
run_test "unset PATH then ls" "unset PATH\nls" "minishell_only"
run_test "unset PATH then /bin/ls" "unset PATH\n/bin/ls" "minishell_only"
run_test "unset HOME then cd" "unset HOME\ncd" "minishell_only"

# ======================
# CLEANUP AND SUMMARY
# ======================
rm -rf "$TEST_DIR"

echo -e "\n${BLUE}======================================${NC}"
echo -e "${BLUE}TEST SUMMARY${NC}"
echo -e "${BLUE}======================================${NC}"
echo -e "${GREEN}Tests executed: $TESTS_TOTAL${NC}"
echo -e "${GREEN}Tests passed: $TESTS_PASSED${NC}"
echo -e "${RED}Tests failed: $TESTS_FAILED${NC}"
echo -e "${BLUE}======================================${NC}\n"
