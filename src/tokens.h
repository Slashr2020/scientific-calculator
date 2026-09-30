#ifndef TOKENS
#define TOKENS

typedef enum {
  TOKEN_OPERAND,
  TOKEN_OPERATOR,
  TOKEN_FUNCTION,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_SPLITTER,
  TOKEN_EOF,
  TOKEN_ERROR
} TokenType;

typedef enum{
  ARITY_UNARY,
  ARITY_BINARY
} Arity;

typedef enum {
  OPERATOR_ADD,
  OPERATOR_SUB,
  OPERATOR_MUL,
  OPERATOR_DIV,
  OPERATOR_UNARY_SUB,
  OPERATOR_POW,
  OPERATOR_MOD,
  OPERATOR_NONE
} OperatorType;

typedef enum{
  FUNCTION_SQRT,
  FUNCTION_SIN,
  FUNCTION_COS,
  FUNCTION_TAN,
  FUNCTION_CTAN,
  FUNCTION_LOG,
  FUNCTION_ABS
} FunctionType;

typedef struct {
  TokenType type;
  union {
    double double_value;
    OperatorType op;
    FunctionType func;
  } value;
} Token;

typedef struct{
  char symbol;
  OperatorType op;
  int priority;
  Arity arity;
} OperatorMap;

typedef struct{
  const char *name;
  FunctionType func;
  Arity arity;
} FunctionMap;

typedef struct{
  const char *name;
  double value;
} ConstantMap;

static const OperatorMap OPERATOR_TABLE[]={
  {'+', OPERATOR_ADD, 1, ARITY_BINARY},
  {'-', OPERATOR_SUB, 1, ARITY_BINARY},
  {'-', OPERATOR_UNARY_SUB, 4, ARITY_UNARY},
  {'*', OPERATOR_MUL, 2, ARITY_BINARY},
  {'^', OPERATOR_POW, 3, ARITY_BINARY},
  {'/', OPERATOR_DIV, 2, ARITY_BINARY},
  {'%', OPERATOR_MOD, 2, ARITY_BINARY}
};

static const FunctionMap FUNCTION_TABLE[]={
  {"sqrt", FUNCTION_SQRT, ARITY_UNARY},
  {"sin", FUNCTION_SIN, ARITY_UNARY},
  {"cos", FUNCTION_COS, ARITY_UNARY},
  {"tan", FUNCTION_TAN, ARITY_UNARY},
  {"ctan", FUNCTION_CTAN, ARITY_UNARY},
  {"log", FUNCTION_LOG, ARITY_BINARY},
  {"abs", FUNCTION_ABS, ARITY_UNARY}
};

static const ConstantMap CONSTANT_TABLE[]={
  {"pi", 3.141592653},
  {"e", 2.718281828}
};

static const unsigned int OPERATOR_TABLE_SIZE = sizeof(OPERATOR_TABLE) / sizeof(OPERATOR_TABLE[0]);
static const unsigned int FUNCTION_TABLE_SIZE = sizeof(FUNCTION_TABLE) / sizeof(FUNCTION_TABLE[0]);
static const unsigned int CONSTANT_TABLE_SIZE = sizeof(CONSTANT_TABLE) / sizeof(CONSTANT_TABLE[0]);

static inline int get_operator_priority(OperatorType op){
  for (unsigned int i=0; i<OPERATOR_TABLE_SIZE; i++){
    if (OPERATOR_TABLE[i].op == op) return OPERATOR_TABLE[i].priority;
  }
  return -1;
}

static inline Arity _get_operator_arity(OperatorType op){
  for (unsigned int i=0; i<OPERATOR_TABLE_SIZE; i++){
    if (OPERATOR_TABLE[i].op == op) return OPERATOR_TABLE[i].arity;
  }
  return ARITY_BINARY;
}

static inline Arity _get_function_arity(FunctionType func){
  for (unsigned int i=0; i<FUNCTION_TABLE_SIZE; i++){
    if (FUNCTION_TABLE[i].func == func) return FUNCTION_TABLE[i].arity;
  }
  return ARITY_BINARY;
}

#define get_arity(X) _Generic((X), OperatorType: _get_operator_arity, FunctionType: _get_function_arity)(X)

Token get_token(const char **expr);

void print_token(Token t);

double calculate(double b, double a, OperatorType operator_type);
double calculate_func(double a, double b, FunctionType function_type);

#endif
