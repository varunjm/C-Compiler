This is a basic Comipler for 'C' programs  

Author : Varun Jayathirtha  
Language used : C++

A) Compilation, Execution

	Compilation	: g++ -Wall -o final final.cpp  
	Execution	: ./final <filename.c>  
				( Provide a 'C' language file name.  
				Example : ./final ./test_input_files/automaton.c )  
	Output		: An output file <Output.c> is generated.

B) Information

This program takes in basic(no structs, pointers) 'C' program as input and generates equivalent of assembly code.

C) High level algorithm

codeGen.cpp
> The parser along with parsing of the code also generates a tree using the natural tree structure of the parser.
> The code generator runs through the tree making necessary changes to the code and emits the code onto a file.

variableRename.cpp
> Final renaming of variables with 'local' or 'global' array is done, which uses a scanner(tokenizer) to do the renaming.
