#include <stddef.h>
#include <stdio.h>
#include "rpn.h"
#include "stack.h"
#include "tokens.h"

double rpn_parser(const char *expr){
  const char *ptr = expr;
  Token t;

  stack operators_stack;
  stack_init(&operators_stack);

  double result;
  stack result_stack;
  stack_init(&result_stack);

  int expend_operand=1;

  do {
    t = get_token(&ptr);
    switch (t.type) {
      case TOKEN_OPERAND:
        print_token(t);
        double_stack_push(&result_stack, t.value.double_value);
        expend_operand=0;
        break;
      case TOKEN_OPERATOR: {
        if (expend_operand==1 && t.value.op == OPERATOR_SUB){
          t.value.op = OPERATOR_UNARY_SUB;
        }

        int current_op_priority = get_operator_priority(t.value.op);
        while (stack_is_empty(&operators_stack)==0 && stack_peek(&operators_stack).type == TOKEN_OPERATOR) {
          OperatorType top_op = stack_peek(&operators_stack).value.op;
          int op_priority = get_operator_priority(top_op);

          int should_pop = (t.value.op == OPERATOR_POW) ? op_priority > current_op_priority : op_priority >= current_op_priority;
          if (should_pop) { 
            Token pop_op = stack_pop(&operators_stack);
            print_token(pop_op);
            double b = double_stack_pop(&result_stack);
            double a = (pop_op.value.op == OPERATOR_UNARY_SUB) ? 0 : double_stack_pop(&result_stack);
            double_stack_push(&result_stack,calculate(a, b, pop_op.value.op));
          }
          else { break; }
        }
        stack_push(&operators_stack, t);
        expend_operand=1;
        break;
      } 
      case TOKEN_FUNCTION: { stack_push(&operators_stack, t); break;}
      case TOKEN_SPLITTER: {
        while (stack_peek(&operators_stack).type!=TOKEN_LPAREN){
          Token pop_op = stack_pop(&operators_stack);
          print_token(pop_op);
          double b = double_stack_pop(&result_stack);
          double a = (get_arity(pop_op.value.op) == ARITY_UNARY) ? 0 : double_stack_pop(&result_stack);
          double_stack_push(&result_stack,calculate(a, b, pop_op.value.op));
        }
        expend_operand=1;
        break;
      }
      case TOKEN_LPAREN: { stack_push(&operators_stack, t); expend_operand=1; break; }
      case TOKEN_RPAREN: 
        while (stack_peek(&operators_stack).type!=TOKEN_LPAREN){
          Token pop_op = stack_pop(&operators_stack);
          print_token(pop_op);
          double b = double_stack_pop(&result_stack);
          double a = (get_arity(pop_op.value.op) == ARITY_UNARY) ? 0 : double_stack_pop(&result_stack);
          double_stack_push(&result_stack,calculate(a, b, pop_op.value.op));
        }
        stack_pop(&operators_stack);

        if (stack_peek(&operators_stack).type==TOKEN_FUNCTION){
          Token func = stack_pop(&operators_stack);
          print_token(func);
          double a = double_stack_pop(&result_stack);
          double b = (get_arity(func.value.func) == ARITY_UNARY) ? 0 : double_stack_pop(&result_stack);
          double_stack_push(&result_stack, calculate_func(a, b, func.value.func));
        }

        expend_operand=0;
        break;
      case TOKEN_ERROR: return -1;
      case TOKEN_EOF: break;
    }

  }while (t.type!=TOKEN_EOF);

  while (stack_is_empty(&operators_stack)==0){
    Token token = stack_pop(&operators_stack);
    double b = double_stack_pop(&result_stack);
    double a = (get_arity(token.value.op) == ARITY_UNARY) ? 0 : double_stack_pop(&result_stack);
    double_stack_push(&result_stack,calculate(a, b, token.value.op));
    print_token(token);
  }

  printf("\n");
  return double_stack_peek(&result_stack);
}
