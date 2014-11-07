#include "eval.h"

long evaluate_operation(long x, char * y, long z);

long eval(mpc_ast_t * ast){
    if(strstr(ast->tag, "number")) { return atoi(ast->contents); }

    char * operator = ast->children[1]->contents;
    long left_operand = eval(ast->children[2]);

    int i = 3;
    while (strstr(ast->children[i]->tag, "expr")){
        left_operand = evaluate_operation(left_operand, operator, eval(ast->children[i]));
        i++;
    }
    return left_operand;
}


long evaluate_operation(long left_operand, char * operator, long right_operand){
    if(strcmp("+", operator) == 0) { return left_operand + right_operand; }
    if(strcmp("-", operator) == 0) { return left_operand - right_operand; }
    if(strcmp("*", operator) == 0) { return left_operand * right_operand; }
    if(strcmp("/", operator) == 0) { return left_operand / right_operand; }
    return 0;
}
