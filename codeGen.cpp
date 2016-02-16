#include <iostream>
#include <fstream>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEN 150
#define reset(x)  strcpy(x,"cs512"); x[6] = '\0';								// Prefix String

using namespace std;

enum tokenTypes {IDENTIFIER=1,NUMBER,RESERVERWORD,SYMBOL,STRING,METASTATEMENT,WHITESPACE};
	
typedef struct node
{
	char value[256];
	char info[256];
	int childrenCount;
	struct node* children[10];
}TOKENS;

TOKENS *tree;

short int newline;
unsigned int NumberOfTokens;
fstream ofile;
int i = 0;
int NoOfVariables, NoOfFunctions, NoOfStatements, TotalVariables, TotalLabels, TotalLines, GlobalVariableBoreder=-2;
char LabelStack[20][20];
int FunctionBoreders[20][2];
int LabelStackTop = -1;
int FunctionCount;
bool Functionbool = false;

char* pop()
{
	if(LabelStackTop==-1)
		return NULL;
	else
		return LabelStack[LabelStackTop--];
}
void push(char *a)
{
	if(LabelStackTop==19)
		return;
	strcpy(LabelStack[++LabelStackTop],a);
	return;
}


int isAl_(char c)									
{
	if ( ( c >= 'a' && c <= 'z') || ( c >= 'A' && c <= 'Z') ||  c == '_' )		// is alphabet or underscore
		return 1;
	return 0;
}
int isAlNum_(char c)															// is alphabet, number or underscore
{
	if ( ( c >= 'a' && c <= 'z') || ( c >= 'A' && c <= 'Z') || ( c >= '0' && c<= '9' ) || c == '_' ) 
		return 1;
	return 0;
}
int isNum (char *c)																// is a valid number
{
	int length = strlen(c);
	int i = 0;
	
	while(i<length)
	{
		if ( c[i] < '0' || c[i] > '9' )
			return 0;
		i++;
	}
	return 1;
}	

void error(int errCode)															// Display appropriate error and exit
{
	switch(errCode)
	{
		case 0 :
			cout<< "Enter one and only one file name. Quitting! \n";
			break;
		case 1:
			cout<< "Invalid string constant . Quitting! \n";
			break;
		case 2:
			cout<< "Unidentified character. Quitting! \n";
			break;
	}	
		exit(0);
}
	
	
class token												
{
	char name[100];
	int type;
	
	public :
	
	void SetToken(char *str, int val)											// Setting a token name and type
	{
		strcpy(name,str);
		type = val;
	}

	int GetTokenType()
	{
		return type;
	}
	
	char* GetTokenName()
	{
		return name;
	}
} T, Tlist[10000];

class Scanner
{

	char inputFile[50], outputFile[50];
	char str[LEN] , temp[LEN];
	
	fstream file;
	fstream fofile;
	token T;
	int length;
	int EndOfFIle ;
	
	public :
	
	Scanner(char *InputFile)
	{
		NumberOfTokens = EndOfFIle = length = 0;
		
		str[0] = '\0';
		reset(temp);
		
		file.open(InputFile,ios::in);
		fofile.open("Output.c",ios::out);
	}
	
	int HasMoreTokens()
	{
		return !EndOfFIle ; 
	}
	
	token GetNextToken()
	{
																				// get next token, reads input file 1 line at a time
		int i, j = 0;
		char token_str[LEN] ;
		string line("");
		
		newline = 0;
		
		if(length == 0)															// reading a new line 
		{
			ofile<<endl;
			getline(file, line);	
			if(file.eof())
			{
				EndOfFIle = 1;
			}
			strncpy(str, line.c_str(), LEN);									// store as character array for easy manipulation
		    length = sizeof(str);
			str[length - 1] = '\0';
		}
		
		for(i=0;i<length;i++)									
		{																			
			while(str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\0')		// Checking for white spaces
			{
				if(str[i] != '\0')												// Print white spaces to output file to retain formatting
					ofile<<str[i];
				i++;
				if(i==length)													// end of line
					break;
			}
			
			if(i==length)														// end of line
				break;
			if ( str[i] >= '0' && str[i] <= '9')								// checking for <number> token
			{
				do
				{	
					token_str[j] = str[i+j];									// copy the token into another char arrray
					j++;
				}while(str[i+j] >= '0' && str[i+j] <= '9');
			
				token_str[j]='\0';
			
				T.SetToken(token_str,NUMBER);
				i = i+j-1;														// move pointer to next token
				NumberOfTokens++;	
				break;														
			}
		
			else if ( ( str[i] == '/' && str[i+1] == '/') || str[i] == '#' )	// checking for <meta statement>
			{
				fofile << str <<endl;
				strcpy(token_str,str+i);										// copy the token into another char arrray
				token_str[strlen(token_str)] = '\0';
				T.SetToken(token_str,METASTATEMENT);
				i = length;														// move pointer to next token
				NumberOfTokens++;
				newline = 1;
			}
																				// checking for <symbol> - 1
			else if ( ( str[i] == '=' && str[i+1] == '=' ) || ( str[i] == '!' && str[i+1] == '=' ) || 
					  ( str[i] == '<' && str[i+1] == '=' ) || ( str[i] == '>' && str[i+1] == '=' ) || 
					  ( str[i] == '&' && str[i+1] == '&' ) || ( str[i] == '|' && str[i+1] == '|' ) )
			{
				token_str[j] = str[i];
				token_str[j+1] = str[i+1];
				token_str[j+2] = '\0';
				T.SetToken(token_str,SYMBOL);
				i++;															// move pointer to next token
				NumberOfTokens++;
				break;														
			}
																				// checking for <symbol> - 2
			else if ( str[i] == '(' || str[i] == ')' || str[i] == '{' || 
					  str[i] == '}' || str[i] == '[' || str[i] == ']' || 
					  str[i] == '+' || str[i] == '-' || str[i] == '*' || 
					  str[i] == '/' || str[i] == ',' || str[i] == ';' ||
					  str[i] == '=' || str[i] == '<' || str[i] == '>')			
			{
				token_str[j] = str[i];
				token_str[j+1] = '\0';
				T.SetToken(token_str,SYMBOL);
				NumberOfTokens++;
				break;														
			}
			
			else if( isAl_(str[i])	)											// checking for identifiers
			{
				do
				{	
					token_str[j] = str[i+j];									// copy the token into another char arrray
					j++;
				}while( isAlNum_(str[i+j]) );
			
				token_str[j]='\0';
																				// checking for <reserved words>
																			
				if( !( strcmp(token_str,"int") && strcmp(token_str,"void") && strcmp(token_str,"if") && 
					strcmp(token_str,"while") && strcmp(token_str,"return") && strcmp(token_str,"read") && 
					strcmp(token_str,"write") && strcmp(token_str,"print") && strcmp(token_str,"continue") && 
					strcmp(token_str,"break") && strcmp(token_str,"binary") && strcmp(token_str,"decimal") ) ) 
				{
					T.SetToken(token_str,RESERVERWORD);
				}	
				else    
				{
					T.SetToken(token_str,IDENTIFIER);
					reset(temp);
				}
				i = i+j-1;														// move pointer to next token
				NumberOfTokens++;
				break;														
			}
			
			else if ( str[i] == '"')											// checking for <strings>
			{
				do
				{	
					token_str[j] = str[i+j];									// copy the token into another char arrray
					j++;
					if(i+j == length)											// end of line
					{
						error(1);
					}
				}while(str[i+j] != '"');
			
				token_str[j]='"';
				token_str[++j]='\0';
				T.SetToken(token_str,STRING);
				i = i+j-1;														// move pointer to next token
				NumberOfTokens++;
				break;														
			}
			
			else																// None of the tokens match i.e. invalid token
			{
				error(2);
			}
		}
		
		i++;
		
		if(i>=length)															// end of line, reset string
		{
			length = 0;
			str[0] = '\0';
		}
		else																	//fetching of token done, more tokens left in the line 
		{
			length = strlen(str+i);				
			strcpy(temp,str+i);																
			strcpy(str,temp);													// removing the fetched token part from the array
			str[length] = '\0';													// for ex.   'int a,b;' -->  ' a,b;' . 
		}
		return T;
	}
};

TOKENS* makeTerminalNode(const char *val)
{
	TOKENS *temp;

	temp = new TOKENS;
	strcpy(temp->value,val);
	temp->childrenCount = 0;

	return temp;
}
TOKENS* makeNonTerminalNode(const char *val, int count)
{
	TOKENS *temp;

	temp = new TOKENS;
	strcpy(temp->value,val);
	temp->childrenCount = count;

	return temp;
}


bool program(TOKENS* &);
bool func_list(TOKENS* &);
bool func(TOKENS* &);
bool func2(TOKENS* &);
bool func_decl(TOKENS* &);
bool type_name(TOKENS* &);
bool type_name2(TOKENS* &);
bool parameter_list(TOKENS* &);
bool void_parameter_list(TOKENS* &);
bool nonempty_list(TOKENS* &);
bool data_decls(TOKENS* &);
bool id_list(TOKENS* &);
bool id_list2(TOKENS* &);
bool id(TOKENS* &);
bool id2(TOKENS* &);
bool block_statement(TOKENS* &);
bool statements(TOKENS* &);
bool statement(TOKENS* &);
bool assignment_or_func_call(TOKENS* &);
bool assignment(TOKENS* &);
bool func_call(TOKENS* &);
bool expr_list(TOKENS* &);
bool nonempty_expr_list(TOKENS* &);
bool nonempty_expr_list2(TOKENS* &);
bool if_statement(TOKENS* &);
bool condition_expression(TOKENS* &);
bool condition_expression2(TOKENS* &);
bool condition_op(TOKENS* &);
bool condition(TOKENS* &);
bool comparison_op(TOKENS* &);
bool while_statement(TOKENS* &);
bool return_statement(TOKENS* &);
bool return_statement2(TOKENS* &);
bool break_statement(TOKENS* &);
bool continue_statement(TOKENS* &);
bool expression(TOKENS* &);
bool expression2(TOKENS* &);
bool addop(TOKENS* &);
bool term(TOKENS* &);
bool term2(TOKENS* &);
bool mulop(TOKENS* &);
bool factor(TOKENS* &);
bool factor2(TOKENS* &);
void printExpressionList(TOKENS* &, char (&)[256]);
void printExpression(TOKENS* &, char (&)[256]);
bool program(TOKENS* &head)
{
	TOKENS *temp1, *temp2;

	if( data_decls(temp1) )
	{
		if( func_list(temp2) && (i == (int)NumberOfTokens) )
		{
			head = makeNonTerminalNode("program",2);
			head->children[0] = temp1;
			head->children[1] = temp2;
			return true;
		}
		
		return false;
	}
	
	return false;
}

bool func_list(TOKENS* &head)
{
	TOKENS *temp1, *temp2;
	if( func(temp1) )
	{
		if ( (i != (int)NumberOfTokens) && func_list(temp2) )
		{
			head = makeNonTerminalNode("func_list",2);
			head->children[0] = temp1;
			head->children[1] = temp2;
			return true;
		}
		else if(i == (int)NumberOfTokens)
		{
			head = makeNonTerminalNode("func_list",1);
			head->children[0] = temp1;
			return true;
		}	
		
	}
	
	return true;
}

bool func(TOKENS* &head)
{
	TOKENS *temp1, *temp2;
	if( func_decl(temp1) )
		if( func2(temp2) )
		{
			head = makeNonTerminalNode("func",2);
			head->children[0] = temp1;
			head->children[1] = temp2;
			return true;
		}
	
	
	return false;
}

bool func2(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3, *temp4;

	if( strcmp(Tlist[i].GetTokenName(),";") == 0 )
	{	
		i++;
		head = makeTerminalNode(";");
		return true;
	}
	else if( strcmp(Tlist[i].GetTokenName(),"{") == 0 )
	{
		i++;
		
		temp1 = makeTerminalNode("{");
		
		if( data_decls(temp2) )
		{
			if( statements(temp3) )
			{
				if( strcmp(Tlist[i].GetTokenName(),"}") == 0 )
				{
					i++;
					NoOfFunctions++;
					
					temp4 = makeTerminalNode("}");
		
					head = makeNonTerminalNode("func2",4);
					head->children[0] = temp1;
					head->children[1] = temp2;
					head->children[2] = temp3;
					head->children[3] = temp4;
					
					return true;
				}
				return false;
			}
			
			return false;
		}
		
		i--;
		return false;
	}
	
	return false;
}

bool func_decl(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3, *temp4, *temp5;
	if( type_name(temp1) )
	{
		if( Tlist[i].GetTokenType() == IDENTIFIER )
		{	
			temp2 = makeTerminalNode(Tlist[i].GetTokenName());
			i++;

			if( strcmp(Tlist[i].GetTokenName(),"(") == 0 )
			{	
				i++;
				temp3 = makeTerminalNode("(");
			
				if( parameter_list(temp4) )
				{
					if( strcmp(Tlist[i].GetTokenName(),")") == 0 )
					{
						i++;
						temp5 = makeTerminalNode(")");				
						head = makeNonTerminalNode("func_decl",5);
						head->children[0] = temp1;	
						head->children[1] = temp2;	
						head->children[2] = temp3;	
						head->children[3] = temp4;	
						head->children[4] = temp5;	
						return true;
					}
					
				}
				
				i--;
			}
			
			i--;
		}
		
	}
	
	return false;
}
	
bool type_name(TOKENS* &head)
{
	if( strcmp(Tlist[i].GetTokenName(),"int") == 0 || strcmp(Tlist[i].GetTokenName(),"void") == 0 || strcmp(Tlist[i].GetTokenName(),"binary") == 0 || strcmp(Tlist[i].GetTokenName(),"decimal") == 0)
	{
		head = makeTerminalNode(Tlist[i].GetTokenName());
		i++;
		return true;
	}
	return false;
}

bool type_name2(TOKENS* &head)
{
	if( strcmp(Tlist[i].GetTokenName(),"int") == 0 || strcmp(Tlist[i].GetTokenName(),"binary") == 0 || strcmp(Tlist[i].GetTokenName(),"decimal") == 0)
	{
		head = makeTerminalNode(Tlist[i].GetTokenName());
		i++;
		return true;
	}
	return false;
}

bool parameter_list(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3;

	if( strcmp(Tlist[i].GetTokenName(),"void") == 0 )
	{
		i++;
		temp1 = makeTerminalNode("void");

		if( void_parameter_list(temp2) )
		{
			head = makeNonTerminalNode("parameter_list",2);
			head->children[0] = temp1;
			head->children[1] = temp2;
			
			return true;
		}
		else
		{
			
			head = makeTerminalNode(Tlist[i].GetTokenName());
			return true;
		}
	}
	if( type_name2(temp1) )
	{
		if( Tlist[i].GetTokenType() == IDENTIFIER )
		{
			temp2 = makeTerminalNode(Tlist[i].GetTokenName());

			i++;
			if( nonempty_list(temp3) )
			{
				head = makeNonTerminalNode("parameter_list",3);
				head->children[0] = temp1;	
				head->children[1] = temp2;	
				head->children[2] = temp3;	
				
				return true;
			}
			
			i--;
			return false;
		}
		
		return false;
	}
	head = NULL;
	return true;
}
	
bool void_parameter_list(TOKENS* &head)
{
	TOKENS *temp1, *temp2;

	if( Tlist[i].GetTokenType() == IDENTIFIER )
	{
		temp1 = makeTerminalNode(Tlist[i].GetTokenName());

		i++;
		if( nonempty_list(temp2) )
		{
			head = makeNonTerminalNode("void_parameter_list",2);
			head->children[0] = temp1; 
			head->children[1] = temp2;
			return true;
		}
		
		i--;
		return false;
	}
	head = NULL;
	return true;
}

bool nonempty_list(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3, *temp4;

	if( strcmp(Tlist[i].GetTokenName(),",") == 0	)
	{
		temp1 = makeTerminalNode(Tlist[i].GetTokenName());

		i++;
		if( type_name(temp2) )
		{
			if( Tlist[i].GetTokenType() == IDENTIFIER )
			{
				temp3 = makeTerminalNode(Tlist[i].GetTokenName());

				i++;
				if( nonempty_list(temp4) )
				{
					head = makeNonTerminalNode("nonempty_list",4);
					head->children[0] = temp1;
					head->children[1] = temp2;
					head->children[2] = temp3;
					head->children[3] = temp4;
					
					return true;
				}
				
				i--;
				return false;
			}
			
			return false;
		}
		
		i--;
		return false;
	}
	head = NULL;
	return true;
}

bool data_decls(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3, *temp4;

	if( strcmp(Tlist[i+2].GetTokenName(),"(") == 0)
	{
		head = NULL;
		return true;
	}	
	
	if( type_name(temp1) )
	{
		if( id_list(temp2) )
		{
			if( strcmp(Tlist[i].GetTokenName(),";") == 0 )
			{
				temp3 = makeTerminalNode(";");
				i++;
				if( data_decls(temp4) )
				{	
					head = makeNonTerminalNode("data_decls",4);
					head->children[0] = temp1;
					head->children[1] = temp2;
					head->children[2] = temp3;
					head->children[3] = temp4;
					return true;
				}
				
				i--;
			}
			
			return false;
		}
		
		return false;
	}
	head = NULL;
	return true;
}

bool id_list(TOKENS* &head)
{
	TOKENS *temp1, *temp2;
	if( id(temp1) )
	{
		NoOfVariables++;
		if( id_list2(temp2) )
		{
			head = makeNonTerminalNode("id_list",2);
			head->children[0] = temp1;
			head->children[1] = temp2;
			return true;
		}
		
	}
	
	return false;
}

bool id_list2(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3;

	if( strcmp(Tlist[i].GetTokenName(),",") == 0	)
	{
		temp1 = makeTerminalNode(",");

		i++;
		if( id(temp2) )
		{
			NoOfVariables++;
			if( id_list2(temp3) )
			{
				head = makeNonTerminalNode("id_list2",3);
				head->children[0] = temp1;
				head->children[1] = temp2;
				head->children[2] = temp3;
				return true;
			}
			
			return false;
		}
		
		i--;
		return false;
	}
	head = NULL;
	return true;
}

bool id(TOKENS* &head)
{
	TOKENS *temp1, *temp2;

	if( Tlist[i].GetTokenType() == IDENTIFIER )
	{	
		temp1 = makeTerminalNode(Tlist[i].GetTokenName());
		i++;
		
		if( id2(temp2) )
		{	
			head = makeNonTerminalNode("id",2);
			head->children[0] = temp1;
			head->children[1] = temp2;
			return true;
		}
		
		i--;
	}
	
	return false;
}

bool id2(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3;

	if( strcmp(Tlist[i].GetTokenName(),"[") == 0 )
	{
		i++;
		temp1 = makeTerminalNode("[");

		if( expression(temp2) )
		{
			if( strcmp(Tlist[i].GetTokenName(),"]") == 0 )
			{
				i++;
				temp3 = makeTerminalNode("]");
				head = makeNonTerminalNode("id2",3);
				head->children[0] = temp1;
				head->children[1] = temp2;
				head->children[2] = temp3;
				return true;
			}
		}
		i--;
		return false;
	}
	head = NULL;
	return true;
}

bool block_statement(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3;

	if( strcmp(Tlist[i].GetTokenName(),"{") == 0 )
	{
		i++;
		temp1 = makeTerminalNode("{");
		if( statements(temp2) )
		{
			if( strcmp(Tlist[i].GetTokenName(),"}") == 0 )
			{
				i++;
				temp3 = makeTerminalNode("}");
				head = makeNonTerminalNode("block_statement",3);
				head->children[0] = temp1;
				head->children[1] = temp2;
				head->children[2] = temp3;
				return true;
			}
			
		}
		i--;
		
	}
	
	return false;
}
	
bool statements(TOKENS* &head)
{
	TOKENS *temp1, *temp2;

	if( statement(temp1) )
	{
		NoOfStatements++;
		if( statements(temp2) )
		{
			head = makeNonTerminalNode("statements",2);
			head->children[0] = temp1;
			head->children[1] = temp2;
			return true;
		}
		
		return false;
	}
	head = NULL;
	return true;
}

bool statement(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3, *temp4, *temp5;
	
	if( Tlist[i].GetTokenType() == IDENTIFIER )
	{
		temp1 = makeTerminalNode(Tlist[i].GetTokenName());
		i++;
		if( assignment_or_func_call(temp2) )
		{
			head = makeNonTerminalNode("statement_1",2);
			head->children[0] = temp1;
			head->children[1] = temp2;
			return true;
		}
		
		i--;
	}
	
	if( if_statement(temp1) )
	{
		
		head = makeNonTerminalNode("statement_2",1);
		head->children[0] = temp1;	
		return true;
	}

	if( while_statement(temp1) )
	{
		
		head = makeNonTerminalNode("statement_3",1);
		head->children[0] = temp1;	
		return true;
	}
	if( return_statement(temp1) )
	{
		
		head = makeNonTerminalNode("statement_4",1);
		head->children[0] = temp1;	
		return true;
	}
	if( break_statement(temp1) )
	{
		
		head = makeNonTerminalNode("statement_5",1);
		head->children[0] = temp1;	
		return true;
	}
	if( continue_statement(temp1) )
	{
		
		head = makeNonTerminalNode("statement_6",1);
		head->children[0] = temp1;	
		return true;
	}
	if( strcmp(Tlist[i].GetTokenName(),"read") == 0 )
	{
		i++;
		
		temp1 = makeTerminalNode("read");
		if( strcmp(Tlist[i].GetTokenName(),"(") == 0 )
		{
			i++;
			temp2 = makeTerminalNode("(");
			if( Tlist[i].GetTokenType() == IDENTIFIER )
			{
				temp3 = makeTerminalNode(Tlist[i].GetTokenName());
				i++;
				if( strcmp(Tlist[i].GetTokenName(),")") == 0 )
				{
					temp4 = makeTerminalNode(")");
					i++;
					if( strcmp(Tlist[i].GetTokenName(),";") == 0 )
					{
						temp5 = makeTerminalNode(";");
						i++;
						head = makeNonTerminalNode("statement_7",5);
						head->children[0] = temp1;
						head->children[1] = temp2;
						head->children[2] = temp3;
						head->children[3] = temp4;
						head->children[4] = temp5;
						return true;
					}
					i--;
					
				}
				i--;
				
			}
			i--;
			
		}
		i--;
		
	}
	

	if( strcmp(Tlist[i].GetTokenName(),"write") == 0 )
	{
		temp1 = makeTerminalNode("write");
		i++;
		if( strcmp(Tlist[i].GetTokenName(),"(") == 0 )
		{
			temp2 = makeTerminalNode("(");
			i++;
			if( expression(temp3) )
			{
				if( strcmp(Tlist[i].GetTokenName(),")") == 0 )
				{
					temp4 = makeTerminalNode(")");
					i++;
					if( strcmp(Tlist[i].GetTokenName(),";") == 0 )
					{
						temp5 = makeTerminalNode(";");
						i++;
						head = makeNonTerminalNode("statement_8",5);
						head->children[0] = temp1;
						head->children[1] = temp2;
						head->children[2] = temp3;
						head->children[3] = temp4;
						head->children[4] = temp5;
						return true;
					}
					
					i--;
				}
				
			}
			
			i--;
		}
		
		i--;
	}
	

	if( strcmp(Tlist[i].GetTokenName(),"print") == 0 )
	{
		temp1 = makeTerminalNode("print");
		i++;
		if( strcmp(Tlist[i].GetTokenName(),"(") == 0 )
		{
			temp2 = makeTerminalNode("(");
			i++;
			if( Tlist[i].GetTokenType() == STRING )
			{
				temp3 = makeTerminalNode(Tlist[i].GetTokenName());
				i++;
				if( strcmp(Tlist[i].GetTokenName(),")") == 0 )
				{
					temp4 = makeTerminalNode(")");
					i++;
					if( strcmp(Tlist[i].GetTokenName(),";") == 0 )
					{
						temp5 = makeTerminalNode(";");
						i++;
						head = makeNonTerminalNode("statement_9",5);
						head->children[0] = temp1;
						head->children[1] = temp2;
						head->children[2] = temp3;
						head->children[3] = temp4;
						head->children[4] = temp5;
						return true;
					}
					
					i--;
				}
				
				i--;
			}
			
			i--;
		}
		
		i--;
	}
	
	return false;
}		

bool assignment_or_func_call(TOKENS* &head)
{
	TOKENS *temp1;

	if( assignment(temp1) )
	{
		head = makeNonTerminalNode("assignment_or_func_call_1",1);
		head->children[0] = temp1;
		return true;
	}
	

	if( func_call(temp1) )
	{
		head = makeNonTerminalNode("assignment_or_func_call_2",1);
		head->children[0] = temp1;
		return true;
	}
	
	return false;
}
	
bool assignment(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3, *temp4;

	if( id2(temp1) )
	{
		if( strcmp(Tlist[i].GetTokenName(),"=") == 0 )
		{
			temp2 = makeTerminalNode("=");
			i++;
			if( expression(temp3) )
			{
				if( strcmp(Tlist[i].GetTokenName(),";") == 0 )
				{
					temp4 = makeTerminalNode(";");
					i++;
					head = makeNonTerminalNode("assignment",4);
					head->children[0] = temp1;
					head->children[1] = temp2;
					head->children[2] = temp3;
					head->children[3] = temp4;
						
					return true;
				}
			}
			i--;
		}
		
	}
	
	return false;
}

bool func_call(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3, *temp4;

	if( strcmp(Tlist[i].GetTokenName(),"(") == 0 )
	{
		temp1 = makeTerminalNode("(");
		i++;
		if( expr_list(temp2) )
		{
			if( strcmp(Tlist[i].GetTokenName(),")") == 0 )
			{
				temp3 = makeTerminalNode(")");
				i++;
				if( strcmp(Tlist[i].GetTokenName(),";") == 0 )
				{
					temp4 = makeTerminalNode(";");
					i++;	
					head = makeNonTerminalNode("func_call",4);
					head->children[0] = temp1;
					head->children[1] = temp2;
					head->children[2] = temp3;
					head->children[3] = temp4;
					
					return true;
				}
				
				i--;
			}
			
		}
		
		i--;
	}
	
	return false;
}

bool expr_list(TOKENS* &head)
{
	TOKENS *temp1;
	if( nonempty_expr_list(temp1) )
	{
		head = makeNonTerminalNode("expr_list",1);
		head->children[0] = temp1;
		return true;
	}
	head = NULL;
	return true;
}

bool nonempty_expr_list(TOKENS* &head)
{
	TOKENS *temp1, *temp2;

	if( expression(temp1) )
		if( nonempty_expr_list2(temp2) )
		{
			head = makeNonTerminalNode("nonempty_expr_list",2);
			head->children[0] = temp1;
			head->children[1] = temp2;
			
			return true;
		}
	
	
	return false;
}
	
bool nonempty_expr_list2(TOKENS* &head) 
{
	TOKENS *temp1, *temp2, *temp3;

	if( strcmp(Tlist[i].GetTokenName(),",") == 0 )
	{
		temp1 = makeTerminalNode(",");
		i++;
		if( expression(temp2) )
		{
			if( nonempty_expr_list2(temp3) )
			{
				head = makeNonTerminalNode("nonempty_expr_list2",3);
				head->children[0] = temp1;
				head->children[1] = temp2;
				head->children[2] = temp3;
					
				return true;
			}
			
			return false;
		}
		
		i--;
		return false;
	}
	head = NULL;
	return true;
}
	
bool if_statement(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3, *temp4, *temp5;

	if( strcmp(Tlist[i].GetTokenName(),"if") == 0 )
	{	
		temp1 = makeTerminalNode("if");
		i++;
		if( strcmp(Tlist[i].GetTokenName(),"(") == 0 )
		{
			temp2 = makeTerminalNode("(");
			i++;
			if( condition_expression(temp3) )
			{
				if( strcmp(Tlist[i].GetTokenName(),")") == 0 )
				{
					temp4 = makeTerminalNode(")");
					i++;
					if( block_statement(temp5) )
					{
						head = makeNonTerminalNode("if_statement",5);
						head->children[0] = temp1;
						head->children[1] = temp2;
						head->children[2] = temp3;
						head->children[3] = temp4;
						head->children[4] = temp5;

						return true;
					}
					i--;
					
				}
				
			}
			i--;
			
		}
		
		i--;
	}
	
	return false;
}

bool condition_expression(TOKENS* &head)
{
	TOKENS *temp1, *temp2;

	if( condition(temp1) )
		if( condition_expression2(temp2) )
		{
			head = makeNonTerminalNode("condition_expression",2);
			head->children[0] = temp1;
			head->children[1] = temp2;

			return true;
		}
	return false;
}

bool condition_expression2(TOKENS* &head)	
{
	TOKENS *temp1, *temp2;

	if( condition_op(temp1) )
	{
		if( condition(temp2) )
		{
			head = makeNonTerminalNode("condition_expression2",2);
			head->children[0] = temp1;
			head->children[1] = temp2;

			return true;
		}
		return false;
	}
	head = NULL;
	return true;
}

bool condition_op(TOKENS* &head)
{
	if( strcmp(Tlist[i].GetTokenName(),"&&") == 0 || strcmp(Tlist[i].GetTokenName(),"||") == 0 )
	{
		head = makeTerminalNode(Tlist[i].GetTokenName());
		i++;
		return true;
	}
	return false;
}

bool condition(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3;

	if( expression(temp1) )
	{
		if( comparison_op(temp2) )
		{
			if( expression(temp3) )
			{
				head = makeNonTerminalNode("condition",3);
				head->children[0] = temp1;
				head->children[1] = temp2;
				head->children[2] = temp3;

				return true;
			}
		}
	}
	return false;
}

bool comparison_op(TOKENS* &head)
{
	if( strcmp(Tlist[i].GetTokenName(),"==") == 0 || strcmp(Tlist[i].GetTokenName(),"!=") == 0 || strcmp(Tlist[i].GetTokenName(),">") == 0 || strcmp(Tlist[i].GetTokenName(),">=") == 0 || strcmp(Tlist[i].GetTokenName(),"<") == 0 || strcmp(Tlist[i].GetTokenName(),"<=") == 0 ) 
	{
		head = makeTerminalNode(Tlist[i].GetTokenName());
		i++;
		return true;
	}
	return false;
}

bool while_statement(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3, *temp4, *temp5;

	if( strcmp(Tlist[i].GetTokenName(),"while") == 0 )
	{
		temp1 = makeTerminalNode("while");
		i++;
		if( strcmp(Tlist[i].GetTokenName(),"(") == 0 )
		{
			temp2 = makeTerminalNode("(");
			i++;
			if( condition_expression(temp3) )
			{
				if( strcmp(Tlist[i].GetTokenName(),")") == 0 )
				{
					temp4 = makeTerminalNode(")");
					i++;
					if( block_statement(temp5) )
					{
						head = makeNonTerminalNode("while_statement",5);
						head->children[0] = temp1;
						head->children[1] = temp2;
						head->children[2] = temp3;
						head->children[3] = temp4;
						head->children[4] = temp5;

						return true;
					}
					i--;
					
				}
				
			}
			
			i--;
		}
		
		i--;
	}
	
	return false;
}

bool return_statement(TOKENS* &head)
{
	TOKENS *temp1, *temp2;

	if( strcmp(Tlist[i].GetTokenName(),"return") == 0 )
	{
		temp1 = makeTerminalNode("return");
		i++;
		if( return_statement2(temp2) )
		{
			head = makeNonTerminalNode("return_statement",2);
			head->children[0] = temp1;
			head->children[1] = temp2;

			return true;
		}
		
		i--;
	}
	
	return false;
}

bool return_statement2(TOKENS* &head)
{
	TOKENS *temp1, *temp2;

	if( expression(temp1) )
	{
		if( strcmp(Tlist[i].GetTokenName(),";") == 0 )
		{
			temp2 = makeTerminalNode(";");
			i++;
			head = makeNonTerminalNode("return_statement2",2);
			head->children[0] = temp1;
			head->children[1] = temp2;
			return true;
		}
		
		return false;
	}
	
	if( strcmp(Tlist[i].GetTokenName(),";") == 0 )
	{
		head = makeTerminalNode(";");
		i++;
		return true;
	}
	return false;
}

bool break_statement(TOKENS* &head)
{
	TOKENS *temp1, *temp2;

	if( strcmp(Tlist[i].GetTokenName(),"break") == 0 )
	{
		temp1 = makeTerminalNode("break");
		i++;
		if( strcmp(Tlist[i].GetTokenName(),";") == 0 )
		{
			temp2 = makeTerminalNode(";");
			i++;
			head = makeNonTerminalNode("break_statement",2);
			head->children[0] = temp1;
			head->children[1] = temp2;

			return true;
		}
		
		i--;
	}
	
	return false;
}

bool continue_statement(TOKENS* &head)
{
	TOKENS *temp1, *temp2;

	if( strcmp(Tlist[i].GetTokenName(),"continue") == 0 )
	{
		i++;
		temp1 = makeTerminalNode("continue");
		if( strcmp(Tlist[i].GetTokenName(),";") == 0 )
		{	
			temp2 = makeTerminalNode(";");
			i++;
			head = makeNonTerminalNode("continue_statement",2);
			head->children[0] = temp1;
			head->children[1] = temp2;

			return true;
		}
		i--;
		
	}
	
	return false;
}

bool expression(TOKENS* &head)
{
	TOKENS *temp1, *temp2;

	if( term(temp1) )
		if( expression2(temp2) )
		{
			head = makeNonTerminalNode("expression",2);
			head->children[0] = temp1;
			head->children[1] = temp2;

			return true;
		}
	
	
	return false;
}

bool expression2(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3;

	if( addop(temp1) )
	{
		if( term(temp2) )
		{
			if( expression2(temp3) )
			{
				head = makeNonTerminalNode("expression2",3);
				head->children[0] = temp1;
				head->children[1] = temp2;
				head->children[2] = temp3;

				return true;
			}
			
			return false;
		}
		
		return false;
	}
	head = NULL;
	return true;
}

bool addop(TOKENS* &head)
{
	if( strcmp(Tlist[i].GetTokenName(),"+") == 0 || strcmp(Tlist[i].GetTokenName(),"-") == 0 )
	{
		head = makeTerminalNode(Tlist[i].GetTokenName());
		i++;
		return true;
	}
	return false;
}

bool term(TOKENS* &head)
{	
	TOKENS *temp1, *temp2;

	if( factor(temp1) )
		if( term2(temp2) )
		{
			head = makeNonTerminalNode("term",2);
			head->children[0] = temp1;
			head->children[1] = temp2;

			return true;
		}
	return false;
}

bool term2(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3;

	if( mulop(temp1) )
	{
		if( factor(temp2) )
		{
			if( term2(temp3) )
			{
				head = makeNonTerminalNode("term2",3);
				head->children[0] = temp1;
				head->children[1] = temp2;
				head->children[2] = temp3;

				return true;
			}
			
			return false;
		}
		
		return false;
	}
	head = NULL;
	return true;	
}

bool mulop(TOKENS* &head)
{
	if( strcmp(Tlist[i].GetTokenName(),"*") == 0 || strcmp(Tlist[i].GetTokenName(),"/") == 0 )
	{
		head = makeTerminalNode(Tlist[i].GetTokenName());
		i++;
		return true;
	}
	return false;
}

bool factor(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3;

	if( Tlist[i].GetTokenType() == IDENTIFIER )
	{
		temp1 = makeTerminalNode(Tlist[i].GetTokenName());
		i++;
		if( factor2(temp2) )
		{
			head = makeNonTerminalNode("factor_1",2);
			head->children[0] = temp1;
			head->children[1] = temp2;

			return true;
		}
		i--;
	}
	if( Tlist[i].GetTokenType() == NUMBER )
	{
		head = makeTerminalNode(Tlist[i].GetTokenName());
		i++;
		return true;
	}
	if( strcmp(Tlist[i].GetTokenName(),"-") == 0 )
	{	
		temp1 = makeTerminalNode("-");
		i++;
		if( Tlist[i].GetTokenType() == NUMBER )
		{
			temp2 = makeTerminalNode(Tlist[i].GetTokenName());
			i++;
			head = makeNonTerminalNode("factor_3",2);
			head->children[0] = temp1;
			head->children[1] = temp2;

			return true;
		}
		i--;
	}
	if( strcmp(Tlist[i].GetTokenName(),"(") == 0 )
	{
		temp1 = makeTerminalNode("(");
		i++;
		if( expression(temp2) )
		{
			if( strcmp(Tlist[i].GetTokenName(),")") == 0 )
			{
				temp3 = makeTerminalNode(")");
				i++;
				head = makeNonTerminalNode("factor_4",3);
				head->children[0] = temp1;
				head->children[1] = temp2;
				head->children[2] = temp3;

				return true;
			}
			
		}
		i--;
	}
	
	return false;
}

bool factor2(TOKENS* &head)
{
	TOKENS *temp1, *temp2, *temp3;

	if( strcmp(Tlist[i].GetTokenName(),"[") == 0 )
	{
		temp1 = makeTerminalNode("[");
		i++;
		if( expression(temp2) )
		{
			if( strcmp(Tlist[i].GetTokenName(),"]") == 0 )
			{
				temp3 = makeTerminalNode("]");
				i++;
				head = makeNonTerminalNode("factor2_1",3);
				head->children[0] = temp1;
				head->children[1] = temp2;
				head->children[2] = temp3;

				return true;
			}
			
			return false;
		}
		
		i--;
		return false;
	}
	
	
	if( strcmp(Tlist[i].GetTokenName(),"(") == 0 )
	{
		temp1 = makeTerminalNode("(");
		i++;
		if( expr_list(temp2) )
		{
			if( strcmp(Tlist[i].GetTokenName(),")") == 0 )
			{
				temp3 = makeTerminalNode(")");
				i++;
				head = makeNonTerminalNode("factor2_2",3);
				head->children[0] = temp1;
				head->children[1] = temp2;
				head->children[2] = temp3;

				return true;
			}
			
			return false;
		}
		i--;
		
		return false;
	}
	head = NULL;
	return true;
}

void implementScanner(char *inFile)
{
	unsigned int PrevCount = 0;
	Scanner scanner(inFile);
	int count = 0;
	
	while ( scanner.HasMoreTokens() )											// dump tokens into output file sequentially
		{ 
			PrevCount = NumberOfTokens;
		
			T = scanner.GetNextToken();
		
			if(PrevCount!=NumberOfTokens )
			{
				ofile<<T.GetTokenName()<<" ";
		   		if( T.GetTokenType() != METASTATEMENT )
		   			Tlist[count++] = T;
		   		newline = 0;
		    }
		}
	   ofile<<endl;
	   NumberOfTokens = count;
}

bool implementParser()
{
	if( program(tree) )																// Begin recursive descent with first production rule
	{
		return true;
	}
	else
	{	
		cout<< "Error\n";
		return false;
	}
}
 
void printExpressionList(TOKENS* &head, char (&exprVar)[256])
{
	char expressionVariable1[256], expressionVariable2[256], expressionVariable3[256], newVariable[256];
	TOKENS* temp,*temp1;

	if(head == NULL)
	{
		strcpy(exprVar,"");
		return;
	}
	else
	{
		temp = head->children[0];
		temp1 = temp->children[0];
		printExpression(temp1,expressionVariable1);
		if( temp1->children[0]->children[0]->childrenCount == 0 && temp1->children[1] == NULL)
		{
			sprintf(newVariable,"xVariable%d",TotalVariables);
			TotalVariables++;
			ofile<<newVariable<<" = "<<expressionVariable1<<";\n";
			TotalLines++;
			strcpy(expressionVariable1,newVariable);
		}
		temp = temp->children[1];
		while(temp != NULL)
		{
			temp1 = temp->children[1];
			printExpression(temp1,expressionVariable2);
			if( temp1->children[0]->children[0]->childrenCount == 0 && temp1->children[1] == NULL)
			{
				sprintf(newVariable,"xVariable%d",TotalVariables);
				TotalVariables++;
				ofile<<newVariable<<" = "<<expressionVariable2<<";\n";
				TotalLines++;
				strcpy(expressionVariable2,newVariable);
			}
			strcat(expressionVariable1,",");
			strcat(expressionVariable1,expressionVariable2);
			temp = temp->children[2];
		}
		strcpy(exprVar,expressionVariable1);
		return;
	}

} 
void printExpression(TOKENS* &head, char (&exprVar)[256])
{
	char expressionVariable1[256], expressionVariable2[256], expressionVariable3[256], newVariable[256];
	TOKENS* temp;

	if(head==NULL)
		return;
	else if( head->childrenCount == 0)
	{
		strcpy(exprVar,head->value);
		return;
	}
	else
	{
		if( strcmp(head->value,"expression")==0 || strcmp(head->value,"term")==0  )
		{
			printExpression(head->children[0],expressionVariable1);
			temp = head->children[1];
			while(temp != NULL)
			{
				printExpression(temp->children[0],expressionVariable2);
				printExpression(temp->children[1],expressionVariable3);
				sprintf(newVariable,"xVariable%d",TotalVariables);
				TotalVariables++;
				ofile<< newVariable << " = " << expressionVariable1 << " " << expressionVariable2 << " " << expressionVariable3 <<" ;\n";
				TotalLines++;
				strcpy(expressionVariable1,newVariable);
				temp = temp->children[2];
			} 
			strcpy(exprVar,expressionVariable1);
			return;
		}
		else if( strcmp(head->value,"factor_1")==0 )
		{
			printExpression(head->children[0],expressionVariable1);
			if( head->children[1] == NULL )
			{
				strcpy(exprVar,expressionVariable1);
				return;
			}
			else
			{
				temp = head->children[1];
				if( strcmp(temp->value,"factor2_1")==0 )
				{
					printExpression(temp->children[1],expressionVariable2);
					sprintf(newVariable,"xVariable%d",TotalVariables);
					TotalVariables++;
					ofile<< newVariable << " = " << expressionVariable1 << "[" << expressionVariable2 << "] ;\n";
					TotalLines++;
				}
				else if( strcmp(temp->value,"factor2_2")==0 )			// Expression list TO BE DONE
				{
					printExpressionList(temp->children[1],expressionVariable2);
					sprintf(newVariable,"xVariable%d",TotalVariables);
					TotalVariables++;
					ofile<< newVariable << " = " << expressionVariable1 << "(" << expressionVariable2 << ") ;\n";
					TotalLines++;
				}
				strcpy(exprVar,newVariable);
				return;
			}
		}
		else if( strcmp(head->value,"factor_3")==0 )
		{
			printExpression(head->children[1],expressionVariable2);
			sprintf(newVariable,"xVariable%d",TotalVariables);
			TotalVariables++;
			ofile<< newVariable << " = -" << expressionVariable2 << ";\n";
			TotalLines++;
			strcpy(exprVar,newVariable);
			return;
		}

		else if( strcmp(head->value,"factor_4")==0 )
		{
			printExpression(head->children[1],expressionVariable2);
			sprintf(newVariable,"xVariable%d",TotalVariables);
			TotalVariables++;
			ofile<< newVariable << " = ( " << expressionVariable2 << " ) ;\n";
			TotalLines++;
			strcpy(exprVar,newVariable);
			return;
		}

		else											//NUMBER
		{
			strcpy(exprVar,head->value);
			return;
		}
	}

}

void printTree(TOKENS* &head)
{
	if(head == NULL)
		return;
	else if(head->childrenCount == 0)
	{
		ofile<< head->value << " ";
		if( strcmp(head->value,";") == 0 ||  strcmp(head->value,"{") == 0 || strcmp(head->value,"}") == 0 )
		{
			ofile<< endl;
			TotalLines++;
		}
		return;
	}
	else
	{
		for(int i=0;i<head->childrenCount;i++)
		{
			printTree(head->children[i]);
		}
		return;
	}
}

void implementCodeGenerator(TOKENS* &head)
{
	char newVariable[256], newLabel[256], newLabel1[256], newLabel2[256];

	if(head == NULL)
		return;
	else if(head->childrenCount == 0)
	{
		ofile<< head->value << " ";
		if( strcmp(head->value,";") == 0 ||  strcmp(head->value,"{") == 0 || strcmp(head->value,"}") == 0 )
		{
			ofile<< endl;
			TotalLines++;
		}
		return;
	}
	else
	{
		int k;
		char expressionVariable1[256], expressionVariable2[256], expressionVariable3[256], expressionVariable4[256];
		TOKENS *temp, *temp1, *temp2, *temp3;

		if( strcmp(head->value,"func_list") == 0)
				Functionbool = true;

		for(k=0;k<head->childrenCount;k++)
		{
			

			temp = head->children[k];
			if (temp == NULL)
				continue; 

			if(strcmp(temp->value,"expression")==0)
			{
				printExpression(temp,expressionVariable1);
				break;
			}
			else if(strcmp(temp->value,"statement_1")==0)
			{
				printExpression(temp->children[0],expressionVariable1);
				temp1 = temp->children[1];

				if(	strcmp(temp1->value,"assignment_or_func_call_1")==0 )		// assignment statement
				{
					if( temp1->children[0]->children[0] == NULL )
					{
						printExpression(temp1->children[0]->children[2],expressionVariable2);
						sprintf(newVariable,"%s = %s;\n",expressionVariable1,expressionVariable2);
						TotalLines++;
						strcpy(expressionVariable1,newVariable);
						break;
					}
					else
					{
						printExpression(temp1->children[0]->children[0]->children[1],expressionVariable2);
						printExpression(temp1->children[0]->children[2],expressionVariable3);
						sprintf(newVariable,"%s[ %s ] = %s;\n",expressionVariable1,expressionVariable2,expressionVariable3);
						TotalLines++;
						strcpy(expressionVariable1,newVariable);
						break;
					}
				}

				if(	strcmp(temp1->value,"assignment_or_func_call_2")==0 )		// function call statement
				{
					temp1 = temp1->children[0]->children[1];
					printExpressionList(temp1,expressionVariable2);
					sprintf(newVariable,"%s( %s );\n",expressionVariable1,expressionVariable2);
					TotalLines++;
					strcpy(expressionVariable1,newVariable);
					break;
				}
			}
			
			else if(strcmp(temp->value,"statement_2")==0)						// If statement
			{
				temp = temp->children[0];
				temp1 = temp->children[2];										// conditional expression
				temp2 = temp1->children[0];										// condi
				temp3 = temp->children[4]->children[1];							// statements
				
				
				if(temp1->children[1] == NULL)
				{
					printExpression(temp2->children[0],expressionVariable1);
					printExpression(temp2->children[1],expressionVariable2);
					printExpression(temp2->children[2],expressionVariable3);
					

					sprintf(newLabel,"label%d",TotalLabels);
					TotalLabels++;
					sprintf(newLabel1,"label%d",TotalLabels);
					TotalLabels++;
					ofile << "if( " << expressionVariable1 << expressionVariable2 << expressionVariable3 << " ) goto "<< newLabel <<";\ngoto "<< newLabel1 << ";\n";
					TotalLines+=2;
					ofile << newLabel << " :;\n";
					TotalLines++;
					implementCodeGenerator(temp3);
					ofile << newLabel1 << " :;\n";
					TotalLines++;
					strcpy(expressionVariable1,"");
					break;
				}
				else
				{
					printExpression(temp2->children[0],expressionVariable1);
					printExpression(temp2->children[1],expressionVariable2);
					printExpression(temp2->children[2],expressionVariable3);
					sprintf(newVariable,"xVariable%d",TotalVariables);
					TotalVariables++;
					ofile << newVariable << " = " << expressionVariable1 << " " << expressionVariable2 << " " << expressionVariable3 <<";\n";
					TotalLines++;
					strcpy(expressionVariable1,newVariable);
					temp1 = temp1->children[1];
					temp2 = temp1->children[1];
					printExpression(temp2->children[0],expressionVariable4);
					printExpression(temp2->children[1],expressionVariable2);
					printExpression(temp2->children[2],expressionVariable3);
					sprintf(newVariable,"xVariable%d",TotalVariables);
					TotalVariables++;
					ofile << newVariable << " = " << expressionVariable4 << " " << expressionVariable2 << " " << expressionVariable3 <<";\n";
					TotalLines++;
					strcpy(expressionVariable4,newVariable);
					printExpression(temp1->children[0],expressionVariable2);
					
					sprintf(newLabel,"label%d",TotalLabels);
					TotalLabels++;
					sprintf(newLabel1,"label%d",TotalLabels);
					TotalLabels++;

					ofile << "if( " << expressionVariable1 << " " << expressionVariable2 << " " << expressionVariable4 <<" ) goto "<< newLabel <<";\ngoto "<< newLabel1 << ";\n";
					ofile << newLabel << " :;\n";
					TotalLines+=3;
					implementCodeGenerator(temp3);
					ofile << newLabel1 << " :;\n";
					TotalLines++;
					strcpy(expressionVariable1,"");
					break;
						
				}
			}
			
			else if(strcmp(temp->value,"statement_3")==0)						// While statement
			{
				
				sprintf(newLabel2,"label%d",TotalLabels);
				TotalLabels++;
				ofile<< newLabel2 << " :;\n";
				TotalLines++;
				sprintf(newLabel,"label%d",TotalLabels);
				TotalLabels++;
				sprintf(newLabel1,"label%d",TotalLabels);
				TotalLabels++;
				push(newLabel2);
				push(newLabel1);
				temp = temp->children[0];
				temp1 = temp->children[2];
				temp2 = temp1->children[0];
				temp3 = temp->children[4]->children[1];
				
				
				
				
				if(temp1->children[1] == NULL)
				{
					printExpression(temp2->children[0],expressionVariable1);
					printExpression(temp2->children[1],expressionVariable2);
					printExpression(temp2->children[2],expressionVariable3);
						
					ofile << "if( " << expressionVariable1 << expressionVariable2 << expressionVariable3 << " ) goto "<< newLabel <<";\ngoto "<< newLabel1 << ";\n";
					ofile << newLabel << " :;\n";
					TotalLines+=3;
				
					implementCodeGenerator(temp3);
					pop();
					pop();
					ofile<< "goto " << newLabel2 << " ;\n";
					ofile << newLabel1 << " :;\n";
					TotalLines+=2;
					strcpy(expressionVariable1,"");
					break;
				}
				else
				{

					printExpression(temp2->children[0],expressionVariable1);
					printExpression(temp2->children[1],expressionVariable2);
					printExpression(temp2->children[2],expressionVariable3);
					sprintf(newVariable,"xVariable%d",TotalVariables);
					TotalVariables++;
					ofile << newVariable << " = " << expressionVariable1 << " " << expressionVariable2 << " " << expressionVariable3 <<";\n";
					TotalLines++;
					strcpy(expressionVariable1,newVariable);	

					temp1 = temp1->children[1];
					temp2 = temp1->children[1];
					printExpression(temp2->children[0],expressionVariable4);
					printExpression(temp2->children[1],expressionVariable2);
					printExpression(temp2->children[2],expressionVariable3);
					sprintf(newVariable,"xVariable%d",TotalVariables);
					TotalVariables++;
					ofile << newVariable << " = " << expressionVariable4 << " " << expressionVariable2 << " " << expressionVariable3 <<";\n";
					TotalLines++;
					strcpy(expressionVariable4,newVariable);
					printExpression(temp1->children[0],expressionVariable2);
					
					ofile << "if( " << expressionVariable1 << " " << expressionVariable2 << " " << expressionVariable4 <<" ) goto "<< newLabel <<";\ngoto "<< newLabel1 << ";\n";
					ofile << newLabel << " :;\n";
					TotalLines+=3;
					implementCodeGenerator(temp3);
					pop();
					pop();
					ofile<< "goto " << newLabel2 << " ;\n";
					ofile << newLabel1 << " :;\n";
					TotalLines+=2;
					strcpy(expressionVariable1,"");
					break;
						
				}
			}

			else if(strcmp(temp->value,"break_statement")==0)					// break statement
			{
				strcpy(expressionVariable1,pop());
				ofile<<"goto "<<expressionVariable1<<" ;\n";
				TotalLines++;
				push(expressionVariable1);
				strcpy(expressionVariable1,"");
				break;
			}
			else if(strcmp(temp->value,"continue_statement")==0)					// break statement
			{
				strcpy(expressionVariable1,pop());
				strcpy(expressionVariable2,pop());
				ofile<<"goto "<<expressionVariable2<<" ;\n";
				TotalLines++;
				push(expressionVariable2);
				push(expressionVariable1);
				strcpy(expressionVariable1,"");
				break;
			}
			else if(strcmp(temp->value,"return_statement")==0)					// Return statement
			{
				temp1 = temp->children[1];

				if(temp1->childrenCount != 0)
				{
					temp1 = temp1->children[0];
					printExpression(temp1,expressionVariable1);
					if( temp1->children[0]->children[0]->childrenCount == 0 && temp1->children[1] == NULL)
					{
						sprintf(newVariable,"xVariable%d",TotalVariables);
						TotalVariables++;
						ofile<<newVariable<<" = "<<expressionVariable1<<";\n";
						TotalLines++;
						strcpy(expressionVariable1,newVariable);
					}	
					sprintf(expressionVariable2,"return %s;\n",expressionVariable1);
					TotalLines++;
					strcpy(expressionVariable1,expressionVariable2);
					break;
				}
				else
				{
					sprintf(expressionVariable1,"return ;\n");
					TotalLines++;
					break;
				}
			}

		}
			
		for(int j=0;j<head->childrenCount;j++)
		{
			if(k==j)
			{
				ofile<<expressionVariable1;	
				continue;
			}
			if( GlobalVariableBoreder ==-2 && Functionbool==false && head->children[j] != NULL && (strcmp(head->children[j]->value,"data_decls") == 0) )
			{
				implementCodeGenerator(head->children[j]);
				GlobalVariableBoreder = TotalLines;
			}
			else if( head->children[j] != NULL && (strcmp(head->children[j]->value,"func2") == 0))
			{
				TotalVariables = 0;
				FunctionBoreders[FunctionCount][0] = TotalLines+1;
				implementCodeGenerator(head->children[j]);
				FunctionBoreders[FunctionCount][1] = TotalLines;
				FunctionCount++;
			}
			else
				implementCodeGenerator(head->children[j]);

		}
		return;
	}
}
 
int main(int argc, char *argv[])
{
	unsigned int i;
	char OutputFile[256];

	if( argc != 2 ) 															// Incomplete input
	{
		error(0);
	}
	
	for( i=0 ; i<strlen(argv[1]) ; i++)											// Generating name of output file from input file name
	{
		if(argv[1][i] == '.')
		{
			OutputFile[i] = '\0';
			strcat(OutputFile,"_gen");
			strcat(OutputFile,argv[1]+i);
			break;
		}
		else if( i+1 == strlen(argv[1]) )										// No Extention
		{
			OutputFile[i] = argv[1][i];
			OutputFile[i+1] = '\0';
			strcat(OutputFile,"_gen");
			break;
		}	
		OutputFile[i] = argv[1][i];
	}
	
	ofile.open(OutputFile,ios::out);											// Open the output file
	implementScanner(argv[1]);													// Tokenize input and store in Tlist[]
	ofile.close();
	
	if (implementParser() )														// Parse the Tokens stored in Tlist[]
	{
		ofile.open("intermediate.txt",ios::out);
		implementCodeGenerator(tree);				// print form parse tree
		ofile.close();

		ofile.open("info.txt",ios::out);
		ofile<<FunctionCount<<endl;
		ofile<<GlobalVariableBoreder<<endl;
		for(int i=0;i<FunctionCount;i++)
		{
			ofile<<FunctionBoreders[i][0]<<" "<<FunctionBoreders[i][1] << endl;  
		}	
		return 0;
	}
	return 1;
		
}