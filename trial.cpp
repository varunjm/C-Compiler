#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<fstream>

using namespace std;

int main()
{
	fstream ifile;
	
	int GlobalVariableBoreder;
	int FunctionBoreders[20][2];
	int j=0, count;


	ifile.open("info.txt",ios::in);
	ifile >> count;
	ifile >> GlobalVariableBoreder;

	while( count-- )
	{
		
		ifile>>FunctionBoreders[j][0];
		ifile>>FunctionBoreders[j][1];
		j++;
	}

	for(int i=0;i<j;i++)
	{
		cout<<FunctionBoreders[i][0]<<" "<<FunctionBoreders[i][1] << endl;  
	}

	return 0;
}