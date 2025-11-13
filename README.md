# Interpreter

Overview

This is a basic lexer (tokenizer) written in C++.
It reads source code and breaks it into tokens like keywords, identifiers, numbers, strings, and symbols.
It’s a simple example of how a lexer works in a compiler or interpreter.

What It Does

Splits code into tokens

Supports:

Keywords like let, if, else, while, print

Strings ("hello")

Numbers (42, 3.14)

Identifiers (variable names)

Operators (+, -, *, /, ==, !=, etc.)

Comments (// comment)

Prints each token with its type, text, and line number

How to Run

Make sure you have a C++17 or newer compiler.

Save the code in a file, for example lexer.cpp.

Compile and run it:

g++ -std=c++17 lexer.cpp -o lexer
./lexer
