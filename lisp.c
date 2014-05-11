#include <stdio.h>
#include <stdlib.h>
#include "lib/mpc.h"
#include "eval.h"

#ifdef _WIN32

#include <string.h>

static char buffer[2048];

char * readline(char * prompt){
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char * copy = malloc(strlen(buffer) + 1);
    copy = strcpy(copy, buffer);
    copy[strlen(buffer+1)] = '\0';
    return copy;
}

void add_history(Char * unused){}

#else

#include <editline/readline.h>

#endif

int main(int argc, char ** argv){

    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expression = mpc_new("expression");
    mpc_parser_t* Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
              "                                                         \
              number    : /-?[0-9]+/;                                   \
              operator  : '+' | '-' | '*' | '/';                        \
              expression: <number> | '(' <operator> <expression>+ ')';  \
              lispy     : /^/ <operator> <expression>+ /$/;             \
              ",
              Number, Operator, Expression, Lispy);

    puts("Lispy Version 0.0.0.0.1");
    puts("Press Ctrl+c to Exit\n");

    while(1){
        char * input = readline("lispy> ");
        add_history(input);

        mpc_result_t result;
        if(mpc_parse("<stdin>", input, Lispy, &result)){
            printf("%li\n", eval(result.output));
            mpc_ast_delete(result.output);
        }else{
            mpc_err_print(result.error);
            mpc_err_delete(result.error);
        }

        free(input);
    }

    mpc_cleanup(4, Number, Operator, Expression, Lispy);
}

