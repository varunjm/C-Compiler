void recursedigit ( int n ) { 
int local[9];
if( 0==n ) goto label0;
goto label1;
label0 :;
return ;
label1 :;
local[0] = 0;
local[2] = n / 2 ;
local[3] = ( local[2] ) ;
local[4] = local[3] * 2 ;
local[5] = ( local[4] ) ;
local[6] = n - local[5] ;
local[7] = ( local[6] ) ;
if( 0!=local[7] ) goto label2;
goto label3;
label2 :;
local[0] = 1;
label3 :;
local[8] = n / 2 ;
recursedigit( local[8] );
if( 0==local[0] ) goto label4;
goto label5;
label4 :;
print ( "0" ) ; 
label5 :;
if( 1==local[0] ) goto label6;
goto label7;
label6 :;
print ( "1" ) ; 
label7 :;
} 
int main ( ) { 
int local[1];
local[0] = 0;
label8 :;
if( 0>=local[0] ) goto label9;
goto label10;
label9 :;
print ( "Give me a number: " ) ; 
read ( local[0] ) ; 
if( 0>=local[0] ) goto label11;
goto label12;
label11 :;
print ( "I need a positive integer.\n" ) ; 
label12 :;
goto label8 ;
label10 :;
print ( "The binary representation of: " ) ; 
write ( local[0]) ; 
print ( "is: " ) ; 
recursedigit( local[0] );
print ( "\n\n" ) ; 
} 

