#!/bin/bash
# ============================================================================
#   minishell differential tester  (v2)
#   Usage:  ./tester.sh [path_to_minishell]      (default: ../minishell)
#
#   Runs every case in BOTH bash and minishell, diffs stdout + exit status.
#   stderr is only shown on failure (wording legitimately differs).
#
#   v2 fixes two bugs from v1:
#     1. exit status of minishell was swallowed by a pipeline -> always 0
#     2. readline echoes the input line when stdin is not a tty; those echoed
#        lines are now filtered out before comparing.
# ============================================================================

MINISHELL="${1:-../minishell}"
TMP="/tmp/mstester.$$"       # sandbox the shells run in (fixtures only)
IO="/tmp/mstester.$$.io"     # scratch files, kept OUT of the sandbox so `ls` is stable
PASS=0
FAIL=0
FAILED_CASES=()

GREEN='\033[0;32m'; RED='\033[0;31m'; YEL='\033[0;33m'; NC='\033[0m'

[ -x "$MINISHELL" ] || { echo "minishell not found at $MINISHELL"; exit 1; }
MINISHELL="$(cd "$(dirname "$MINISHELL")" && pwd)/$(basename "$MINISHELL")"

mkdir -p "$TMP" "$IO" && cd "$TMP" || exit 1

# ---- fixtures --------------------------------------------------------------
printf 'alpha\nbeta\ngamma\ndelta\n'      > file1.txt
printf 'one two three\nfour five six\n'   > file2.txt
printf 'x\n'                              > small.txt
mkdir -p subdir
echo "nested" > subdir/nested.txt
touch noperm.txt && chmod 000 noperm.txt

# ---- echo/prompt filter ----------------------------------------------------
# minishell (via readline) prints "minishell$ " / "> " and echoes each input
# line back when stdin is not a terminal. Strip the prompts, then drop the
# echoed input lines by matching them against in.txt in order.
strip_echo()
{
	awk '
	NR == FNR { inp[FNR] = $0; n = FNR; next }
	{
		line = $0
		gsub(/minishell\$ /, "", line)
		if (i < n && line == inp[i + 1]) { i++; next }
		if (i < n && substr(line, 1, 2) == "> " && substr(line, 3) == inp[i + 1]) { i++; next }
		print line
	}
	' "$IO/in.txt" "$IO/m.raw"
}

# ---- runner ----------------------------------------------------------------
run_case()
{
	local cmd="$1"
	local b_out b_code m_out m_code

	printf '%s\n' "$cmd" > "$IO/in.txt"

	bash --norc --noprofile < "$IO/in.txt" > "$IO/b.raw" 2> "$IO/b.err"
	b_code=$?
	"$MINISHELL" < "$IO/in.txt" > "$IO/m.raw" 2> "$IO/m.err"
	m_code=$?

	b_out="$(cat "$IO/b.raw")"
	m_out="$(strip_echo)"

	if [ "$b_out" == "$m_out" ] && [ "$b_code" == "$m_code" ]; then
		PASS=$((PASS + 1))
		printf "${GREEN}OK${NC}   %s\n" "$(printf '%s' "$cmd" | tr '\n' ';')"
	else
		FAIL=$((FAIL + 1))
		FAILED_CASES+=("$cmd")
		printf "${RED}KO${NC}   %s\n" "$(printf '%s' "$cmd" | tr '\n' ';')"
		printf "       ${YEL}bash${NC}      out=[%s] code=%s\n" "$b_out" "$b_code"
		printf "       ${YEL}minishell${NC} out=[%s] code=%s\n" "$m_out" "$m_code"
		[ -s "$IO/b.err" ] && printf "       bash  err: %s\n" "$(head -1 "$IO/b.err")"
		[ -s "$IO/m.err" ] && printf "       mini  err: %s\n" "$(head -1 "$IO/m.err")"
	fi
}

section() { printf "\n${YEL}=== %s ===${NC}\n" "$1"; }

# ---- cases -----------------------------------------------------------------
section "simple commands"
run_case '/bin/ls'
run_case '/bin/pwd'
run_case 'pwd'
run_case 'ls'
run_case ''
run_case '     '
run_case 'echo hello'

section "arguments"
run_case '/bin/echo hello world'
run_case 'ls -l file1.txt'
run_case 'wc -l file1.txt'
run_case 'head -2 file1.txt'
run_case 'grep beta file1.txt'

section "echo"
run_case 'echo'
run_case 'echo -n hello'
run_case 'echo -nnn hello'
run_case 'echo -n -n hello'
run_case 'echo -nx hello'
run_case 'echo hello    world'
run_case 'echo "hello    world"'
run_case 'echo - n'

section "exit status"
run_case 'ls file1.txt
echo $?'
run_case 'ls nosuchfile
echo $?'
run_case 'dsbksdgbksdghsd
echo $?'
run_case 'expr 1 + 1'
run_case 'ls nosuchfile
expr $? + $?'
run_case 'echo $?$?'
run_case '/bin/
echo $?'
run_case 'cat noperm.txt
echo $?'

section "double quotes"
run_case 'echo "cat lol.c | cat > lol.c"'
run_case 'echo "hello"world"!"'
run_case 'echo ""'
run_case 'echo "$USER"'
run_case 'echo "a"'"'"'b'"'"'"c"'
run_case '"ls" file1.txt'

section "single quotes"
run_case "echo '\$USER'"
run_case "echo '\$?'"
run_case "echo ''"
run_case "echo '' ''"
run_case "echo 'a'b'c'"
run_case "echo '\"\$USER\"'"
run_case "echo \"'\$USER'\""

section "env / export / unset"
run_case 'export TEST=hello
echo $TEST'
run_case 'export TEST=hello
export TEST=world
echo $TEST'
run_case 'export TEST=hello
unset TEST
echo $TEST'
run_case 'unset NOSUCHVAR
echo $?'
run_case 'export A=1 B=2 C=3
echo $A$B$C'
run_case 'export 1BAD=x
echo $?'
run_case 'export EMPTY=
echo "[$EMPTY]"'
run_case 'export NOVAL
env | grep -c "^NOVAL"
echo done'
run_case 'export SPACED="a b"
echo $SPACED'
run_case 'export SPACED="a   b"
echo "$SPACED"'

section "cd / pwd"
run_case 'cd /tmp
pwd'
run_case 'cd /tmp
cd ..
pwd'
run_case 'cd .
pwd'
run_case 'cd /nosuchdir
echo $?'
run_case 'cd file1.txt
echo $?'
run_case 'cd /tmp
echo $PWD'
run_case 'cd subdir
pwd'
run_case 'cd /
cd tmp
pwd'

section "PATH"
run_case 'unset PATH
ls
echo $?'
run_case 'export PATH=/bin
echo $?'
run_case 'export PATH=""
ls
echo $?'

section "redirections"
run_case 'echo hello > o1.txt
cat o1.txt'
run_case 'echo a > o2.txt
echo b > o2.txt
cat o2.txt'
run_case 'echo a > o3.txt
echo b >> o3.txt
cat o3.txt'
run_case 'cat < file1.txt'
run_case 'wc -l < file1.txt'
run_case 'cat < nosuchfile
echo $?'
run_case 'echo a > o4.txt > o5.txt
cat o4.txt
cat o5.txt'
run_case 'cat < file1.txt < file2.txt'
run_case '< file1.txt cat'
run_case '> o6.txt
ls -l o6.txt | wc -l'
run_case 'echo test > "sp ace.txt"
cat "sp ace.txt"'

section "syntax errors (standalone: bash aborts the script, so no trailing cmd)"
run_case 'echo x >>'
run_case 'ls > <'
run_case 'ls | | ls'
run_case '| ls'

section "heredoc"
run_case 'cat << EOF
hello
world
EOF'
run_case 'cat << EOF
$USER
EOF'
run_case 'cat << "EOF"
$USER
EOF'
run_case "cat << 'EOF'
\$USER \$?
EOF"
run_case 'cat << EOF > hd.txt
line
EOF
cat hd.txt'
run_case 'cat << E1
first
E1
cat << E2
second
E2'
run_case 'cat << EOF | grep hel
hello
bye
EOF'
run_case 'grep a << EOF
apple
berry
EOF'

section "pipes"
run_case 'ls | wc -l'
run_case 'cat file1.txt | grep a | wc -l'
run_case 'cat file1.txt | head -2'
run_case 'ls nosuchfile | grep bla | wc -l'
run_case 'echo hi | cat | cat | cat'
run_case 'ls | nosuchcmd
echo $?'
run_case 'nosuchcmd | echo hi
echo $?'
run_case 'pwd | pwd'
run_case 'export PIPEVAR=1 | echo x
echo "[$PIPEVAR]"'
run_case 'cd /tmp | echo x
pwd'
run_case 'yes | head -3'
run_case 'cat < file1.txt | grep a > po.txt
cat po.txt'
run_case 'echo hello | cat > pc.txt | cat
cat pc.txt'
run_case 'cat file1.txt | cat | cat | cat | cat | cat | cat | wc -l'

section "environment variables"
run_case 'echo $USER'
run_case 'echo $NOSUCHVARIABLE'
run_case 'echo abc$NOSUCHVARIABLEdef'
run_case 'echo $HOME'
run_case 'echo $USER$USER'
run_case 'echo "$USER"suffix'
run_case 'export V="a b c"
echo $V'
run_case 'export V="  sp  "
echo "$V"'
run_case 'export F=vf.txt
echo hi > $F
cat vf.txt'
run_case 'export CMD=ls
$CMD file1.txt'
run_case 'echo $'
run_case 'echo "$"'
run_case 'echo $ USER'

section "exit builtin"
run_case 'exit 42'
run_case 'exit 300'
run_case 'exit -1'
run_case 'exit abc'
run_case 'exit 1 2'
run_case 'exit'
run_case 'exit 9223372036854775808'
run_case 'exit +5'
run_case 'exit 256'
run_case 'ls nosuchfile
exit'

# ---- report ----------------------------------------------------------------
printf "\n=============================================\n"
printf " ${GREEN}PASSED: %d${NC}   ${RED}FAILED: %d${NC}\n" "$PASS" "$FAIL"
printf "=============================================\n"
if [ "$FAIL" -gt 0 ]; then
	printf "\nFailed cases:\n"
	i=0
	while [ $i -lt ${#FAILED_CASES[@]} ]; do
		printf "  - %s\n" "$(printf '%s' "${FAILED_CASES[$i]}" | tr '\n' ';')"
		i=$((i + 1))
	done
fi

cd / && chmod 777 "$TMP"/noperm.txt 2>/dev/null; rm -rf "$TMP" "$IO"
exit $((FAIL > 0))
