#!/bin/bash

set -u

BASE_PATH="$(dirname "$0")/../../.."
cd "$BASE_PATH"

GREEN='\033[0;32m'
RED='\033[0;31m'
OFF='\033[0m'
STATUS=0

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

COMPILER=".build/Flex-Bison-Compiler"

if [ ! -x "$COMPILER" ]; then
	echo -e "${RED}Compiler not found at $COMPILER. Run src/main/bash/build.sh first.${OFF}"
	exit 1
fi

echo "Code generation should succeed for accept tests..."
echo ""

for test in $(ls src/test/c/accept/); do
	OUTPUT="$TMP_DIR/${test%.ezd}.ino"
	cat "src/test/c/accept/$test" | "$COMPILER" -o "$OUTPUT" >/dev/null 2>&1
	RESULT="$?"
	if [ "$RESULT" == "0" ] && [ -s "$OUTPUT" ]; then
		if grep -q "void setup()" "$OUTPUT" && grep -q "void loop()" "$OUTPUT"; then
			echo -e "    $test, ${GREEN}generated valid .ino${OFF}"
		else
			STATUS=1
			echo -e "    $test, ${RED}missing setup()/loop()${OFF}"
		fi
	else
		STATUS=1
		echo -e "    $test, ${RED}generation failed${OFF} (status $RESULT)"
	fi
done

echo ""
echo "All done."
exit $STATUS
