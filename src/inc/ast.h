#ifndef VALT_AS_H
#define VALT_AS_H

typedef enum { OP_NOT, OP_INC, OP_DEC } UnaryOpType;
typedef enum { OP_SUB, OP_ADD, OP_MUL, OP_DEV, OP_MOD } BinOpType;
typedef enum { BOOL_TRUE, BOOL_FALSE } BoolLit;

typedef struct Expr Expr;

typedef enum {
  EXPR_UNARY,
  EXPR_BINARY,
  EXPR_INT_LITERAL,
  EXPR_FLOAT_LITERAL,
  EXPR_STRING_LITERAL,
  EXPR_CHAR_LITERAL,
  EXPR_BOOL_LITERAL,
  EXPR_IDENTIFIER,
  EXPR_FUNC_CALL,
} ExprType;

typedef struct {
  UnaryOpType op_type;
  Expr *right;
} UnaryExpr;

typedef struct {
  Expr *left;
  BinOpType op_type;
  Expr *right;
} BinExpr;

typedef struct {
  char *value;
} IntLitExpr;

typedef struct {
  char *value;
} FloatLitExpr;

typedef struct {
  char *value;
} StringLitExpr;

typedef struct {
  char *value;
} CharLitExpr;

typedef struct {
  BoolLit boolean;
} BoolLitExpr;

typedef struct {
  char *ident;
} IdenExpr;

struct Expr {
  ExprType expr_type;
  union {
    UnaryExpr unary_expr;
    BinExpr bin_expr;
    IntLitExpr int_lit_expr;
    FloatLitExpr float_lit_expr;
    StringLitExpr str_lit_expr;
    CharLitExpr char_lit_expr;
    BoolLitExpr bool_lit_expr;
    IdenExpr iden_expr;
  };
};

UnaryExpr *getUnaryExpr(UnaryOpType op_type, Expr *right);
BinExpr *getBinExpr(Expr *left, BinOpType op_type, Expr *right);
IntLitExpr *getIntLitExpr(char *value);
FloatLitExpr *getFloatLitExpr(char *value);
StringLitExpr *getStrLitExpr(char *value);
CharLitExpr *getCharLitExpr(char *value);
BoolLitExpr *getBoolLitExpr(BoolLit lit);

#endif