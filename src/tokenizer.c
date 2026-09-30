#include <stdio.h>
#include <ctype.h>

typedef enum {
  TOKEN_OPERAND,
  TOKEN_OPERATOR,
  TOKEN_FUNCTION,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_EOF,
  TOKEN_ERROR
} TokenType;

typedef enum {
  OPERATOR_ADD,
  OPERATOR_SUB,
  OPERATOR_MUL,
  OPERATOR_DIV,
  OPERATOR_NONE
} OperatorType;

typedef enum{
  SQRT,
  SIN,
  COS,
  TAN,
  CTAN,
  FACTORIAL,
  SUM,
  PROD
} FunctionType;

typedef struct {
  TokenType type;
  union {
    int num_value;
    OperatorType operator;
  } value;
} Token;

typedef struct{
  char symbol;
  OperatorType operator;
  int priority;
} OperatorMap;

static const OperatorMap OPERATOR_TABLE[]={
  {'+', OPERATOR_ADD, 1},
  {'-', OPERATOR_SUB, 1},
  {'*', OPERATOR_MUL, 2},
  {'/', OPERATOR_DIV, 2}
};

static const unsigned int OPERATOR_TABLE_SIZE = sizeof(OPERATOR_TABLE) / sizeof(OPERATOR_TABLE[0]);

int get_operator_priority(OperatorType operator){
  for (unsigned int i=0; i<OPERATOR_TABLE_SIZE; i++){
    if (OPERATOR_TABLE[i].operator == operator) return OPERATOR_TABLE[i].priority;
  }

  return -1;
}

Token get_token(const char **expr){
  Token token; token.value.num_value=0;

  while (**expr!='\0' && isspace(**expr)){
    (*expr)++;
  }

  if (**expr=='\0'){
    token.type=TOKEN_EOF;
    return token;
  }

  if (isdigit(**expr)){
    token.type =TOKEN_OPERAND;
    while (**expr!='\0' && isdigit(**expr)) {
      token.value.num_value=token.value.num_value*10+(**expr-'0');
      (*expr)++;
    }
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
      token.value.operator = OPERATOR_TABLE[i].operator;
      (*expr)++;
      return token;
    }
  }

  token.type=TOKEN_ERROR;
  (*expr)++;
  return token;
}
int main(){
  const char *expr = "15 * 3 + 5";
  const char *ptr = expr;

  printf("%s\n",expr);

  Token t;
  do {
    t=get_token(&ptr);

    switch (t.type){
      case TOKEN_LPAREN : printf("VAL ( TYPE PAREN\n"); break;
      case TOKEN_RPAREN : printf("VAL ) TYPE PAREN\n"); break;
      case TOKEN_OPERAND: printf("VAL %d TYPE NUM\n", t.value.num_value); break;
      case TOKEN_OPERATOR: {
        int operator_priority = get_operator_priority(t.value.operator);
        printf("VAL %d PRIOR %d TYPE OPERATOR SYM ", t.value.operator, operator_priority);
        switch (t.value.operator) {
          case OPERATOR_ADD: printf("+\n"); break;
          case OPERATOR_MUL: printf("*\n"); break;
          case OPERATOR_SUB: printf("-\n"); break;
          case OPERATOR_DIV: printf("/\n"); break;
          default: printf("?\n");
        }
      } break;
      case TOKEN_EOF: printf("VAL EOF TYPE EOF\n"); break;
      case TOKEN_ERROR: printf("VAL ERROR TYPE ERROR\n"); break;

    }
  }while (t.type!=TOKEN_EOF && t.type!=TOKEN_ERROR);
  return 0;
}
