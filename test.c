#include <stdio.h>

struct abc {
  int a;
  double b;
};
typedef struct abc abc;

int main(int argc, char* argv[]) {
  abc newstruct;
  newstruct.a = 5;
  newstruct.b = 2;
  printf("newstruct.a = %d\n", newstruct.a);
  printf("newstruct.b = %f\n", newstruct.b);
  return 0;
}
