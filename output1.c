#include <stdio.h>
#define read(x) scanf("%d",&x)
#define write(x) printf("%d\n",x)

int add(int a) {
  int local[3];
  read(local[0]);
  local[1] = a;
  local[2] = local[1]+local[0];
  return local[2];
}

int main() {
  int local[2];
  read(local[0]);
  local[1] = add(local[0]);
  write(local[1]);
}
