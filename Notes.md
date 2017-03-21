## High level information:
  - Reads C programs, but limited grammar. Does not include pointers, structures.
  - Checks for grammatical, but not syntactical correctness.
  - Produces code similar to machine level instructions. 
  
## Three phases
  
  ### Scanner (tokenizer)
  Based on regular expressions for all the tokens that will be identified.
  
  ### Parser
  Based on the grammar for C language, the tokens are parsed to check for syntax correctness. This is an LL(1) recursive
  decsent parser, which have functions defined for each 'non-terminal' that tried to match the tokens with a grammar 
  consisting of a string of terminal and non terminal terms. It effectively translates the program plain text to a tree,
  if it has a valid grammar. 
  
  ### Code Generator
  Using the syntax tree from the previous phase, the code generator generates code similar to machine level instructions.
  Which means
  - No, decision making statements like IF, WHILE, FOR etc. All of these statements are replaced by jump instructions.
  - All variables are grouped and declared as an array of memory locations.
  - Complex statements are seperated out to into 3 operand statements.
                
