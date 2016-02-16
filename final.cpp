#include<iostream>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

int main(int argc, char* argv[])
{
	char temp[256];
	if( argc != 2 ) 															// Incomplete input
	{
		return 0;
	}

	system("g++ -o codeGen codeGen.cpp");
	system("g++ -o variableRename variableRename.cpp");
	sprintf(temp,"./codeGen %s",argv[1]);
	if( system(temp) == 0 )
	{
		system("./variableRename");
		system("cat out >> Output.c");
		system("rm info.txt *_gen* out codeGen variableRename intermediate.txt");
	}
	return 0;
}
