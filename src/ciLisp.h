
//ok to make helper functions if needed
#ifndef __cilisp_h_
#define __cilisp_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "ciLispParser.h"

int yyparse(void);

int yylex(void);

void yyerror(char *);



// Enum of all operators.
// must be in sync with funcs in resolveFunc()
typedef enum oper {
    NEG_OPER, // 0
    ABS_OPER,
    EXP_OPER,
    SQRT_OPER,
    ADD_OPER,
    SUB_OPER,
    MULT_OPER,
    DIV_OPER,
    REMAINDER_OPER,
    LOG_OPER,
    POW_OPER,
    MAX_OPER,
    MIN_OPER,
    EXP2_OPER,
    CBRT_OPER,
    HYPOT_OPER,
    READ_OPER,
    RAND_OPER,
    PRINT_OPER,
    EQUAL_OPER,
    LESS_OPER,
    GREATER_OPER,
    CUSTOM_OPER =255
} OPER_TYPE;

OPER_TYPE resolveFunc(char *);

// Types of Abstract Syntax Tree nodes.
// Initially, there are only numbers and functions.
// You will expand this enum as you build the project.
typedef enum {

    NUM_NODE_TYPE,
    FUNC_NODE_TYPE,
    SYMBOL_NODE_TYPE,
    COND_NODE_TYPE
} AST_NODE_TYPE;

// Types of numeric values
typedef enum {
    INT_TYPE,
    DOUBLE_TYPE
} NUM_TYPE;

typedef struct {
    struct ast_node *cond;
    struct ast_node *trueCond; // to eval if cond is nonzero
    struct ast_node *falseCond; // to eval if cond is zero
} COND_AST_NODE;

typedef enum { VARIABLE_TYPE, LAMBDA_TYPE, ARG_TYPE } SYMBOL_TYPE;

typedef struct stack_node {
    struct ast_node *val;
    struct stack_node *next;
} STACK_NODE;

//Variable Nodes "let_section"
typedef struct symbol_table_node {
    SYMBOL_TYPE type;
    NUM_TYPE val_type;
    char *ident;
    struct ast_node *val;
    STACK_NODE *stack;
    struct symbol_table_node *next;
} SYMBOL_TABLE_NODE;

typedef struct symbol_ast_node {
    char *ident;
} SYMBOL_AST_NODE;

// Node to store a number.
// could remove union and just truncate a double
// may get rounding errors
// make a helper functions
typedef struct {
    NUM_TYPE type;
    double value;
    /*union{
        double dval;
        long ival;
    } value;*/
} NUM_AST_NODE;

// Values returned by eval function will be numbers with a type.
// They have the same structure as a NUM_AST_NODE.
// The line below allows us to give this struct another name for readability.
typedef NUM_AST_NODE RET_VAL;

// Node to store a function call with its inputs
typedef struct {
    OPER_TYPE oper;
    char* ident; // only needed for custom functions
    struct ast_node *opList;
} FUNC_AST_NODE;

// Generic Abstract Syntax Tree node. Stores the type of node,
// and reference to the corresponding specific node (initially a number or function call).
typedef struct ast_node {
    AST_NODE_TYPE type;
    SYMBOL_TABLE_NODE *symbolTable;
    struct ast_node *parent;
    union {
        NUM_AST_NODE number;
        FUNC_AST_NODE function;
        COND_AST_NODE condition;
        SYMBOL_AST_NODE symbol;
    } data;
    struct  ast_node *next;
} AST_NODE;

AST_NODE *createNumberNode( char *typeName, double value);

AST_NODE *createSymbolNode(char *symbolName);
//AST_NODE contains a symbol table node
SYMBOL_TABLE_NODE *createSymbolTableNode(char *symbol, AST_NODE *exprNode, char *typeName, SYMBOL_TABLE_NODE *argNode);
SYMBOL_TABLE_NODE *createSymbolArgNode(char *symbol, SYMBOL_TABLE_NODE *argNode);
//Can make added value the new head or make the tail.
//if new head you can get the the last created variable
SYMBOL_TABLE_NODE *addSymbolToList(SYMBOL_TABLE_NODE *curHead, SYMBOL_TABLE_NODE *newElem);
AST_NODE *parentToAstNode(SYMBOL_TABLE_NODE *symbolNode, AST_NODE *parentASTNode);
//Set parents here
AST_NODE *createFunctionNode(char *funcName, AST_NODE *op1List);
AST_NODE *addFunctionNodeToList(AST_NODE *nextOp, AST_NODE *curOpList);

AST_NODE *createConditionalNode(AST_NODE *condNode, AST_NODE *trueNode, AST_NODE *falseNode);

void freeNode(AST_NODE *node);

RET_VAL eval(AST_NODE *node);
RET_VAL evalNumNode(NUM_AST_NODE *numNode);
RET_VAL evalFuncNode(FUNC_AST_NODE *funcNode);
RET_VAL evalSymbolNode(AST_NODE *symbolNode);
RET_VAL evalConditionalNode(AST_NODE *condNode);

void printRetVal(RET_VAL val);

//HELPER FUNCTIONS
bool checkNumberType(double val);
RET_VAL checkReadNumber();
NUM_TYPE evalType(char *type);
int checkFunctionOpList(FUNC_AST_NODE *funcNode);

#endif
