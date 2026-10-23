#!/bin/sh

set -eu

repository_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$repository_root"

required_files="
Makefile
README.md
resources/data.rsrc
resources/icon.png
resources/repliman.rsrc
src/main.cpp
src/replimanApp.cpp
src/replimanWindow.cpp
"

for required_file in $required_files; do
	if [ ! -f "$required_file" ]; then
		echo "Missing required file: $required_file" >&2
		exit 1
	fi
done

git diff --check

if grep -ERn 'kBlack|kWhite|kHighlight|kMedGrey|kFirstGreen|kSecondGreen|SetViewColor\([[:space:]]*[0-9]' src; then
	echo "Active sources contain legacy hard-coded interface colours." >&2
	exit 1
fi

if [ "$(uname -s)" = "Haiku" ] && command -v findpaths >/dev/null 2>&1; then
	make clean
	make
else
	echo "Repository checks passed; Haiku build skipped on $(uname -s)."
fi
