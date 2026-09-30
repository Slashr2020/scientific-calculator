#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"
#include <math.h>

Token get_token(const char **expr){
  Token token; token.value.double_value=0;

  while (**expr!='\0' && isspace(**expr)){
    (*expr)++;
  }

  if (**expr=='\0'){
    token.type=TOKEN_EOF;
    return token;
  }

  if (isdigit(**expr) || **expr=='.'){
    token.type =TOKEN_OPERAND;
    char *next_ptr;
    token.value.double_value=strtod(*expr, &next_ptr);
    *expr=next_ptr;
    return token;
  }
  
  if (**expr == '('){
    token.type = TOKEN_LPAREN;
    (*expr)++;
    return token;
  }

  if (**expr == ')'){
    token.type = TOKEN_RPAREN;
    (*expr)++;
    return token;
  }

  for (unsigned int i=0; i<OPERATOR_TABLE_SIZE; i++){
    if (**expr == OPERATOR_TABLE[i].symbol){
      token.type = TOKEN_OPERATOR;
      token.value.op = OPERATOR_TABLE[i].op;
      (*expr)++;
      return token;
    }
  }

  for (unsigned int i=0; i<FUNCTION_TABLE_SIZE; i++){
    size_t len = strlen(FUNCTION_TABLE[i].name);
    if (strncmp(*expr, FUNCTION_TABLE[i].name, len)==0){
      token.type = TOKEN_FUNCTION;
      token.value.func = FUNCTION_TABLE[i].func;
      (*expr)+=len;
      return token;
    }
  }

  for (unsigned int i=0; i<CONSTANT_TABLE_SIZE; i++){
    size_t len = strlen(CONSTANT_TABLE[i].name);
    if (strncmp(*expr, CONSTANT_TABLE[i].name, len)==0){
      token.type = TOKEN_OPERAND;
      token.value.double_value = CONSTANT_TABLE[i].value;
      (*expr)+=len;
      return token;
    }
  }

  if (**expr == ','){
    token.type = TOKEN_SPLITTER;
    (*expr)++;
    return token;
  }

  token.type=TOKEN_ERROR;
  (*expr)++;
  return token;
}

double calculate(double b, double a, OperatorType operator_type){
  switch (operator_type) {
    case OPERATOR_ADD: return a + b; 
    case OPERATOR_SUB: return b - a;
    case OPERATOR_MUL: return a * b;
    case OPERATOR_UNARY_SUB: return -a;
    case OPERATOR_POW: {
      if (a> -1e-9 && a<1e-9) return 0.0;
      return pow(b, a);
    }
    case OPERATOR_DIV:
      if (a> -1e-9 && a<1e-9){
        return 0.0;
      }
      return b / a;
    case OPERATOR_MOD:
      if (a==0) return 0.0 / 0.0;
      return fmod(b, a);
#if 0
      long long base = (long long)(b/a);
      return b - (double)base * a;
#endif
    default: return 0.0;
  }
}

double calculate_func(double a, double b, FunctionType function_type){
  switch (function_type) {
    case FUNCTION_SQRT: return sqrt(a);
    case FUNCTION_SIN: return sin(a);
    case FUNCTION_COS: return cos(a);
    case FUNCTION_TAN: return tan(a);
    case FUNCTION_CTAN: return 1/tan(a);
    case FUNCTION_LOG: return log(a)/log(b);
    case FUNCTION_ABS: return fabs(a);
    default: return 0.0;
  }
}

void print_token(Token t){
  switch (t.type){
      case TOKEN_LPAREN : printf("VAL ( TYPE PAREN\n"); break;
      case TOKEN_RPAREN : printf("VAL ) TYPE PAREN\n"); break;
      case TOKEN_OPERAND: {
        printf("VAL %g TYPE NUM\n", t.value.double_value); 
      } break; 
      case TOKEN_OPERATOR: {
        int operator_priority = get_operator_priority(t.value.op);
        printf("VAL %d PRIOR %d TYPE OPERATOR SYM ", t.value.op, operator_priority);
        switch (t.value.op) {
          case OPERATOR_ADD: printf("+\n"); break;
          case OPERATOR_MUL: printf("*\n"); break;
          case OPERATOR_SUB: printf("-\n"); break;
          case OPERATOR_DIV: printf("/\n"); break;
          case OPERATOR_MOD: printf("%%\n"); break;
          case OPERATOR_UNARY_SUB: printf("_\n"); break;
          case OPERATOR_POW: printf("^\n"); break;
          default: printf("?\n");
        } 
        break;
      } 
      case TOKEN_FUNCTION: printf("VAL %d TYPE FUNC\n", t.value.func);
      case TOKEN_SPLITTER: printf("VAL , TYPE SPLITTER\n"); break;
      case TOKEN_EOF: printf("VAL EOF TYPE EOF\n"); break;
      case TOKEN_ERROR: printf("VAL ERROR TYPE ERROR\n"); break;
    }
}
