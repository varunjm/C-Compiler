Readme file for the implementation of a Parser of 'C' programs  
 
Author : Varun Jayathirtha  
Student ID : 200106498  
Language used : C++ 

A) Compilation, Execution and Errors 

	Compilation	: g++ -Wall -o final final.cpp  
	Execution	: ./final <filename.c>  
				( Provide a 'C' language file name.  
				Example : ./final automaton.c )  
	Output		: An output file <Output.c> is generated. 
				  OR
				  "Error"

B) Functionality and description 

The code covers all given description in the question.
The program uses system() calls to clear out intermediate files that were generated. A possible scenario for failure could be running the program in an environment where such calls are not allowed to make. Although the program has been tested on a VCL machine and it works without any issues.

C) High level algorithm

codeGen.cpp
> The parser along with parsing of the code also generates a tree using the natural tree structure of the parser.
> The code generater runs through the tree making necessary changes to the code and emits the code onto a file. 

variableRename.cpp
> Final renaming of variables with 'local' or 'global' array is done, which uses a slightly modified version of the scanner to do the renaming.


