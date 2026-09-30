#ifndef STACK
#define STACK
#define MAX_STACK_SIZE 32
#include "tokens.h"

typedef enum {
  STACK_TYPE_TOKEN,
  STACK_TYPE_VALUE
} StackDataType;

typedef struct{
  StackDataType type; 
  union {
    Token token;
    double real;
  } value; 
}stack_item;

typedef struct{
  stack_item data[MAX_STACK_SIZE];
  int top;
}stack;

void stack_init(stack *s);
int stack_is_empty(stack *s);
int stack_is_full(stack *s);
/* for tokens */
int stack_push(stack *s, Token value);
Token stack_pop(stack *s);
Token stack_peek(stack *s);

/* for values */
int double_stack_push(stack *s, double value);
double double_stack_pop(stack *s);
double double_stack_peek(stack *s);

void stack_print(stack *s);

#endif
