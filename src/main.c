#include <math.h>
#include <stdio.h>
#include "rpn.h"

int main(int argc, char *argv[]){
  const char *expr = argv[1];
  printf("%s\n",expr);
  double res = rpn_parser(expr);
  if (fabs(res)<1e-9) res = 0.0;
  printf("%g\n", res);
  return 0;
}
