#include "stack.h"
#include <stdio.h>
#include "tokens.h"

void stack_init(stack *s){
  s->top=-1;
}

int stack_is_empty(stack *s){
  return (s->top==-1) ? 1 : 0;
}

int stack_is_full(stack *s){
  return (s->top==MAX_STACK_SIZE-1) ? 1 : 0;
}

int stack_push(stack *s, Token value){
  if (stack_is_full(s)==0){
    s->top++;
    s->data[s->top].type = STACK_TYPE_TOKEN;
    s->data[s->top].value.token = value;
    return 0;
  }
  return -1;
}

Token stack_pop(stack *s){
  if (stack_is_empty(s)==1){
    Token error = { .type = TOKEN_ERROR };
    return  error;
  }

  return s->data[(s->top)--].value.token;
}

Token stack_peek(stack *s){
  if (stack_is_empty(s)==0) return s->data[s->top].value.token;
  Token error = {.type = TOKEN_ERROR};
  return error;
}

int double_stack_push(stack *s, double value){
  if (stack_is_full(s)==0){
    s->top++;
    s->data[s->top].type = STACK_TYPE_VALUE;
    s->data[s->top].value.real = value;
    return 0;
  }
  return -1;
}

double double_stack_pop(stack *s){
  if (stack_is_empty(s)==1){
    return -1;
  }

  return s->data[(s->top)--].value.real;
}

double double_stack_peek(stack *s){
  if (stack_is_empty(s)==0) return s->data[s->top].value.real;
  return -1;
}

void stack_print(stack *s){
  if (stack_is_empty(s)==1) return;

  for (int i=s->top; i>=0; i--){
    print_token(s->data[i].value.token);
  }
  printf("\n");
}
