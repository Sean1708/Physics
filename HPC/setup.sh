#!/usr/bin/env bash

# Obtain question number and nickname for project.
read -p 'Question: ' question
read -p 'Name: ' name

mkdir "Q$question"
cd "Q$question"

# C
mkdir 'C'
echo "CFLAGS = -Weverything --std=c11

debug: CFLAGS += -g -DDEBUG -Werror -Wno-unused-parameter
debug: all

release: CFLAGS += -Ofast -DNDEBUG

all: $name

$name: ${name}.c

.PHONY: clean
clean:
	rm -rf $name ${name}.dSYM" > 'C/Makefile'
echo "$name" > 'C/.gitignore'
echo "#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]) {
    printf(\"Hello %s!\\n\", argv[0]);

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

\\renewcommand{\\familydefault}{\\sfdefault}

\\begin{document}
\\title{Question $question: $name}
\\author{Sean Marshallsay: sm1183}
\\date{}
\\maketitle
\\end{document}" > "WriteUp/Q${question}_sm1183.ltx"
