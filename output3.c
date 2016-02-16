#include <stdio.h>
#define read(x) scanf("%d",&x)
#define write(x) printf("%d\n",x)
#define print(x) printf(x)
int g ( ) {
int local[1];
local[0] = 1 ;
return local [ 0 ] ;
}
int f ( ) {
int local[3];
local[0] = g ( ) ;
local[1] = local [ 0 ] + 1 ;
local[2] = local [ 1 ] ;
return local [ 2 ] ;
}
int e ( ) {
int local[3];
local[0] = f ( ) ;
local[1] = local [ 0 ] + 1 ;
local[2] = local [ 1 ] ;
return local [ 2 ] ;
}
int d ( ) {
int local[3];
local[0] = e ( ) ;
local[1] = local [ 0 ] + 1 ;
local[2] = local [ 1 ] ;
return local [ 2 ] ;
}
int c ( ) {
int local[3];
local[0] = d ( ) ;
local[1] = local [ 0 ] + 1 ;
local[2] = local [ 1 ] ;
return local [ 2 ] ;
}
int b ( ) {
int local[3];
local[0] = c ( ) ;
local[1] = local [ 0 ] + 1 ;
local[2] = local [ 1 ] ;
return local [ 2 ] ;
}
int a ( ) {
int local[3];
local[0] = b ( ) ;
local[1] = local [ 0 ] + 1 ;
local[2] = local [ 1 ] ;
return local [ 2 ] ;
}
int main ( ) {
int local[2];
local[1] = a ( ) ;
local [ 0 ] = local [ 1 ] ;
print ( "I calculate the answer to be: " ) ;
write ( local [ 0 ] ) ;
}
