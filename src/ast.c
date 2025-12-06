#include "inc/ast.h"

#include <stdlib.h>

UnaryExpr *getUnaryExpr(UnaryOpType op_type, Expr *right) {
  UnaryExpr *unary_expr = (UnaryExpr *)malloc(sizeof(UnaryExpr));

  unary_expr->op_type = op_type;
  unary_expr->right = right;

  return unary_expr;
}

BinExpr *getBinExpr(Expr *left, BinOpType op_type, Expr *right) {
  BinExpr *bin_expr = (BinExpr *)malloc(sizeof(BinExpr));

  bin_expr->left = left;
  bin_expr->op_type = op_type;
  bin_expr->right = right;

  return bin_expr;
}

IntLitExpr *getIntLitExpr(char *value) {
  IntLitExpr *int_lit_expr = (IntLitExpr *)malloc(sizeof(IntLitExpr));
  int_lit_expr->value = value;
  return int_lit_expr;
}

FloatLitExpr *getFloatLitExpr(char *value) {
  FloatLitExpr *float_lit_expr = (FloatLitExpr *)malloc(sizeof(FloatLitExpr));
  float_lit_expr->value = value;
  return float_lit_expr;
}

StringLitExpr *getStrLitExpr(char *value) {
  StringLitExpr *str_lit_expr = (StringLitExpr *)malloc(sizeof(StringLitExpr));
  str_lit_expr->value = value;
  return str_lit_expr;
}

CharLitExpr *getCharLitExpr(char *value) {
  CharLitExpr *char_lit_expr = (CharLitExpr *)malloc(sizeof(CharLitExpr));
  char_lit_expr->value = value;
  return char_lit_expr;
}

BoolLitExpr *getBoolLitExpr(BoolLit lit) {
  BoolLitExpr *bool_lit_expr = (BoolLitExpr *)malloc(sizeof(BoolLitExpr));
  bool_lit_expr->boolean = lit;
  return bool_lit_expr;
}

IdenExpr *getIdenLitExpr(char *value) {
  IdenExpr *ident_expr = (IdenExpr *)malloc(sizeof(IdenExpr));
  ident_expr->ident = value;
  return ident_expr;
}
