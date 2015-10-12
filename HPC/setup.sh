#!/usr/bin/env bash

# Obtain week number and nickname for project.
read -p 'Week: ' week
read -p 'Name: ' name

mkdir "Week$week"
cd "Week$week"

# C
mkdir 'C'
echo "CFLAGS = -Weverything -Os

debug: CFLAGS += -g -DDEBUG -Werror -Wno-unused-parameter
debug: release

release: $name

$name: ${name}.c

.PHONY: clean
clean:
	rm -rf $name ${name}.dSYM" > 'C/Makefile'
echo "$name" > 'C/.gitignore'
echo "#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]) {
    printf(\"Hello %s!\", argv[0]);

    return 0;
}" > "C/${name}.c"

# Rust
cargo new --bin --vcs=none "$name"
mv "$name" 'Rust'

# TODO: Julia

# LaTeX
mkdir 'WriteUp'
echo "\\documentclass[11pt,a4paper]{article}

\\usepackage[UKenglish]{babel}
\\usepackage{graphicx}

\\begin{document}
\\title{Week$week: $name}
\\author{
    Sean Marshallsay\\\\
    Exam Number: Y0071170
}
\date{}
\maketitle
\\end{document}" > "WriteUp/${name}.ltx"
