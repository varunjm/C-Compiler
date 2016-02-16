#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#define LEN 150
#define reset(x)  strcpy(x,"cs512"); x[6] = '\0';								// Prefix String

using namespace std;

enum tokenTypes {IDENTIFIER=1,NUMBER,RESERVERWORD,SYMBOL,STRING,METASTATEMENT,WHITESPACE};
	
short int newline;
unsigned int NumberOfTokens;
fstream ofile;
short int LineCount;
char variableMap[20][100][256];
int variableCount[20];
char globalVariableMap[100][256];
int globalVariableCount;
char parameterVariableMap[20][100][256];
int parameterVariableCount[20];
int FunctionCount;

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
			cout << "Invalid string constant . Quitting! \n";
			break;
		case 2:
			cout << "Unidentified character. Quitting! \n";
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
} ;

class Scanner
{

	char inputFile[50], outputFile[50];
	char str[LEN] , temp[LEN];
	bool printNewLine;
	fstream file;
	token T;
	int length;
	int EndOfFIle ;
	

	public :
	
	Scanner(char *InputFile)
	{
		NumberOfTokens = EndOfFIle = length = 0;
		printNewLine = false;
		str[0] = '\0';
		reset(temp);
		
		file.open(InputFile,ios::in);
		
	}
	
	int HasMoreTokens()
	{
		return !EndOfFIle ; 
	}
	
	void newLine()
	{
		length = 0;
		printNewLine = false;
	}
	char getNextCharacter()
	{
		int j=0;
		while((int)str[j]<= 32) j++;
		return str[j];
	}
	token GetNextToken()
	{
																				// get next token, reads input file 1 line at a time
		int i,j = 0;
		char token_str[LEN] ;
		string line("");
		
		newline = 0;
		
		if(length == 0)															// reading a new line 
		{
			if(printNewLine)
				ofile<<endl;
			printNewLine = true;
			getline(file, line);	
			LineCount++;
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
					  str[i] == ':' || str[i] == '=' || str[i] == '<' || str[i] == '>')			
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
					strcmp(token_str,"goto") && strcmp(token_str,"return") && strcmp(token_str,"read") && 
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

int isInList(char *a)										// variable already in symbol table
{
	int i;

	for(i=1;i<parameterVariableCount[FunctionCount];i++)
	{
		if( !strcmp(parameterVariableMap[FunctionCount][i],a) )
		{
			return 1;
		}
	}
	for(i=0;i<variableCount[FunctionCount];i++)
	{
		if( !strcmp(variableMap[FunctionCount][i],a) )
		{
			return 2;
		}
	}
	for(i=0;i<globalVariableCount;i++)
	{
		if( !strcmp(globalVariableMap[i],a) )
		{
			return 0;
		}
	}
	return -1;
}

void insertIntoList(char *a, int choice, int size)			// insert variable name into symbol table
{
	switch(choice)
	{
		case 0:													// Global variable List
			if(!size)
				strcpy(globalVariableMap[globalVariableCount++],a);
			else
			{
				strcpy(globalVariableMap[globalVariableCount++],a);
				for(int i=0;i<size-1;i++)
					strcpy(globalVariableMap[globalVariableCount++],"_");
			}
		break;

		case 1:													// Function Parameter List	
			
			if(!size)
				strcpy(parameterVariableMap[FunctionCount][parameterVariableCount[FunctionCount]++],a);	
			else
			{
				strcpy(parameterVariableMap[FunctionCount][parameterVariableCount[FunctionCount]++],a);
				for(int i=0;i<size-1;i++)
					strcpy(parameterVariableMap[FunctionCount][parameterVariableCount[FunctionCount]++],"_");
			}
		break;

		case 2:													// Local variable List
			if(!size)
				strcpy(variableMap[FunctionCount][variableCount[FunctionCount]++],a);	
			else
			{
				strcpy(variableMap[FunctionCount][variableCount[FunctionCount]++],a);
				for(int i=0;i<size-1;i++)
					strcpy(variableMap[FunctionCount][variableCount[FunctionCount]++],"_");
			}

		break;
	}
	
}

void alternateName(char (&a)[256], char index[256])					// Rename it with appropriate array (local or global) name
{
	char temp[256],temp1[256],temp2[256];
	int i;

	for(i=0;i<parameterVariableCount[FunctionCount];i++)
	{
		if( !strcmp(parameterVariableMap[FunctionCount][i],a) )
		{
			if( !strcmp(parameterVariableMap[FunctionCount][i+1],"_") )
				sprintf(temp,"%s[%s]",a,index);	
			else
				sprintf(temp,"%s",a);
			strcpy(a,temp);
			return;
		}
	}
	for(i=0;i<variableCount[FunctionCount];i++)
	{
		if( !strcmp(variableMap[FunctionCount][i],a) )
		{
			if( !strcmp(variableMap[FunctionCount][i+1],"_") )
			{
				strcpy(temp1,index);
				strcpy(temp,"");
				alternateName(temp1,temp);
				sprintf(temp2,"local[%d + %s]",i,temp1);	
				strcpy(temp,temp2);
			}
			else
				sprintf(temp,"local[%d]",i);
			strcpy(a,temp);
			return;
		}
	}
	for(i=0;i<globalVariableCount;i++)
	{
		if( !strcmp(globalVariableMap[i],a) )
		{
			if( !strcmp(globalVariableMap[i+1],"_") )
				sprintf(temp,"global[%d + %s]",i,index);	
			else
				sprintf(temp,"global[%d]",i);
			strcpy(a,temp);
			return ;
		}
	}
}
int main(int argc, char* argv[])
{
	unsigned int i,j=0;
	char inputFile[256] = "intermediate.txt";
	char OutputFile[256] = "out";
	char temp[256], temp1[256];
	unsigned int PrevCount = 0, count;
	fstream ifile;
	token T;
	int FunctionBorders[20][2];
	int GlobalVariableBoreder;

	FunctionCount=0;

	ifile.open("info.txt",ios::in);
	ifile >> count;
	ifile >> GlobalVariableBoreder;

	while( count-- )													// read meta data produced by codeGen after forming the tree
	{
		
		ifile>>FunctionBorders[j][0];
		ifile>>FunctionBorders[j][1];
		j++;
	}

	argv[1] = inputFile;
	
	Scanner scanner1(argv[1]);
	Scanner scanner2(argv[1]);
    
	ofile.open(OutputFile,ios::out);

	while ( scanner1.HasMoreTokens() )											// read and record all variable names
	{ 
		PrevCount = NumberOfTokens;
		T = scanner1.GetNextToken();
		
		if(PrevCount!=NumberOfTokens )
		{
			if (T.GetTokenType()==METASTATEMENT)	continue;
			else if (T.GetTokenType()==IDENTIFIER && (strcmp(T.GetTokenName(),"main") != 0) && (strstr(T.GetTokenName(),"label")==NULL) && scanner1.getNextCharacter() != '(')
       		{	
       			if(LineCount <= GlobalVariableBoreder)							// Insert into list of global variables
       			{
       				if(scanner1.getNextCharacter() == '[')
       				{	
       					strcpy(temp1,T.GetTokenName());
       					T = scanner1.GetNextToken();
       					T = scanner1.GetNextToken();
       					insertIntoList( temp1, 0, atoi(T.GetTokenName()) );
       				}	
       				else
       					insertIntoList( T.GetTokenName(), 0, 0 );						
       			}
     			if( LineCount == FunctionBorders[FunctionCount][0] )			// Insert into list of parameter variables
       			{       				
       				if(scanner1.getNextCharacter() == '[')
       				{	
       					strcpy(temp1,T.GetTokenName());
       					T = scanner1.GetNextToken();
       					T = scanner1.GetNextToken();
       					insertIntoList( temp1, 1, atoi(T.GetTokenName()) );
       				}	
       				else
       					insertIntoList( T.GetTokenName(), 1, 0 );						
       			}
       			if( LineCount > FunctionBorders[FunctionCount][0] && LineCount <= FunctionBorders[FunctionCount][1] )
				{																// Insert into list of local variables
					if( isInList( T.GetTokenName() ) != 2 && isInList( T.GetTokenName() ) != 0)		
					{
						if(scanner1.getNextCharacter() == '[')
	       				{
	       					strcpy(temp1,T.GetTokenName());
	       					T = scanner1.GetNextToken();
	       					T = scanner1.GetNextToken();
	       					insertIntoList( temp1, 2, atoi(T.GetTokenName()) );
	       				}	
	       				else
	       				{
	       					insertIntoList( T.GetTokenName(), 2, 0 );
	       				}
	       			}
	       		}		
			}	
			else if(strcmp(T.GetTokenName(),"}")==0)
       		{
       			FunctionCount++;
       		}
       }
       
    }

    ofile.close();
    ofile.open(OutputFile,ios::out);											// Open the output file
    bool flag1 = true, flag2 = true;;
    LineCount = 0;
    FunctionCount = 0;
    while ( scanner2.HasMoreTokens() )											// dump tokens with renamed variable into output file sequentially
	{ 
		PrevCount = NumberOfTokens;
		T = scanner2.GetNextToken();
		
		if(PrevCount!=NumberOfTokens )
		{
			if(LineCount <= GlobalVariableBoreder && flag2)
			{
				while (strcmp(T.GetTokenName(),"int") == 0 )					//global declaration
				{
					scanner2.newLine();
					T = scanner2.GetNextToken();
					if(LineCount > GlobalVariableBoreder) break;
				}
				flag2 = false;
				ofile << "int global[" << globalVariableCount << "];\n";	
			}
			if(LineCount > FunctionBorders[FunctionCount][0] && flag1)
			{
				while (strcmp(T.GetTokenName(),"int") == 0 )					// local declaration
				{
					scanner2.newLine();
					T = scanner2.GetNextToken();
				}
				flag1 = false;
				ofile << "int local[" << variableCount[FunctionCount] << "];\n";
			}

			strcpy(temp,T.GetTokenName());

			if (LineCount != FunctionBorders[FunctionCount][0] && T.GetTokenType()==IDENTIFIER && (strcmp(temp,"main") != 0) && LineCount > FunctionBorders[FunctionCount][0] && LineCount <= FunctionBorders[FunctionCount][1] && (strstr(temp,"label")==NULL) && scanner2.getNextCharacter() != '(')
       		{ 																	// all variable accesses
       			if(scanner2.getNextCharacter() == '[')
       			{																// special action for array access
       				T = scanner2.GetNextToken();
	       			T = scanner2.GetNextToken();
       				strcpy(temp1,T.GetTokenName());
       				T = scanner2.GetNextToken();
       			}
       			else
       			{
       				strcpy(temp1,"");
       			}
       			alternateName(temp,temp1);										// obtain renamed variable
       			ofile << temp ;
			}
       		else
	       	{	
	       		if(strstr(temp,"}")!=NULL)
       			{
       				FunctionCount++;
       				flag1 = true;
       			}
       			ofile << temp ;
	       	}
       		newline = 0;
        }
    }
   ofile<<endl;
   ofile.close();
}
