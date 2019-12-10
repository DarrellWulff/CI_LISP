#include "ciLisp.h"

void yyerror(char *s) {
    fprintf(stderr, "\nERROR: %s\n", s);
    // note stderr that normally defaults to stdout, but can be redirected: ./src 2> src.log
    // CLion will display stderr in a different color from stdin and stdout
}

// Array of string values for operations.
// Must be in sync with funcs in the OPER_TYPE enum in order for resolveFunc to work.
char *funcNames[] = {
        "neg",
        "abs",
        "exp",
        "sqrt",
        "add",
        "sub",
        "mult",
        "div",
        "remainder",
        "log",
        "pow",
        "max",
        "min",
        "exp2",
        "cbrt",
        "hypot",
        "read",
        "rand",
        "print",
        "equal",
        "less",
        "greater",
        ""
};

char *nodeNames[] = {

        "Number Node",
        "Function Node",
        "Symbol Node",
        "Conditional Node"
};


OPER_TYPE resolveFunc(char *funcName)
{
    int i = 0;
    while (funcNames[i][0] != '\0')
    {
        if (strcmp(funcNames[i], funcName) == 0)
            return i;
        i++;
    }
    return CUSTOM_OPER;
}
// Task 2 you will have to worry about parent nodes
// Called when an INT or DOUBLE token is encountered (see ciLisp.l and ciLisp.y).
// Creates an AST_NODE for the number.
// Sets the AST_NODE's type to number.
// Populates the value of the contained NUMBER_AST_NODE with the argument value.
// SEE: AST_NODE, NUM_AST_NODE, AST_NODE_TYPE.
AST_NODE *createNumberNode( char *typeName, double value)
{
    //populate the ast node
    AST_NODE *node;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    // TODO set the AST_NODE's type, assign values to contained NUM_AST_NODE
    node->type = NUM_NODE_TYPE;
    if(typeName == NULL)
    {
        if(checkNumberType(value))
        {
            node->data.number.value = floor(value);
            node->data.number.type = INT_TYPE;
        }
        else
        {
            node->data.number.value = value;
            node->data.number.type = DOUBLE_TYPE;
        }
    }
    else
        {
            node->data.number.type = evalType(typeName);
            //node->data.symbol.ident = malloc(sizeof(char)*strlen(typeName));
            //strcpy(node->data.symbol.ident, typeName);

            switch (node->data.number.type)
            {
                case INT_TYPE:
                    node->data.number.value = floor(value);
                    break;
                case DOUBLE_TYPE:
                    node->data.number.value = value;
                    break;
                default:
                    printf("\nNumber Node creation went wrong\n");
                    break;
            }
        }

    return node;
}

AST_NODE *createSymbolNode(char *symbolName)
{
    //populate the ast node
    AST_NODE *node;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    node->type = SYMBOL_NODE_TYPE;

    node->data.symbol.ident = malloc(sizeof(char)*strlen(symbolName));
    strcpy(node->data.symbol.ident, symbolName);

    return node;
}

SYMBOL_TABLE_NODE *createSymbolTableNode(char *symbol, AST_NODE *exprNode, char *typeName, SYMBOL_TABLE_NODE *argNode)
{
    //populate the symbol table node
    SYMBOL_TABLE_NODE *node;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(SYMBOL_TABLE_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");


    node->ident = malloc(sizeof(char)*strlen(symbol));
    strcpy(node->ident, symbol);

    node->val = exprNode;

    if(argNode != NULL)
    {
        node->type = LAMBDA_TYPE;
        node->val_type = evalType(typeName);
        STACK_NODE *stackHead;

        nodeSize = sizeof(STACK_NODE);
        if ((stackHead = calloc(nodeSize, 1)) == NULL)
            yyerror("Memory allocation failed!");

        //Link the arguments to the function for eval
        stackHead = argNode->stack;
        node->stack = stackHead;
    }
    else
        {
            node->type = VARIABLE_TYPE;
        }

    if(node->val->type == NUM_NODE_TYPE)
    {
        if(typeName == NULL)
        {
            node->val_type = node->val->data.number.type;
        }
        else
            {
                //Block for casting of a let section.
                //Check if double and see if casting as int to give warning message.
                node->val_type = evalType(typeName);
                node->val->data.number.type = node->val_type;
                if(node->val_type == INT_TYPE && !checkNumberType(node->val->data.number.value))
                {
                    node->val->data.number.value = floor(node->val->data.number.value);
                    printf("\nWARNING: precision loss in the assignment for variable %s\n", symbol);
                }

            }
    }

    return node;
}
//Creates the function argument nodes
SYMBOL_TABLE_NODE *createSymbolArgNode(char *symbol, SYMBOL_TABLE_NODE *argNode)
{

    SYMBOL_TABLE_NODE *node;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(SYMBOL_TABLE_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    STACK_NODE *curStackNode;

    nodeSize = sizeof(STACK_NODE);
    if ((curStackNode = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    //Set up arguments to plug in values later
    node->ident = malloc(sizeof(char)*strlen(symbol));
    strcpy(node->ident, symbol);

    node->val = NULL;
    node->type = ARG_TYPE;
    node->stack = curStackNode;
    if(argNode != NULL)
        node->stack->next = argNode->stack;

    return node;
}

//Push old elements backward making the head the tail in LIFO order
SYMBOL_TABLE_NODE *addSymbolToList(SYMBOL_TABLE_NODE *curHead, SYMBOL_TABLE_NODE *newElem)
{

    if(newElem == NULL) {
        curHead->next = NULL;
    }
    else
        {
            curHead->next = newElem;
        }

    return curHead;
}

AST_NODE *parentToAstNode(SYMBOL_TABLE_NODE *symbolNode, AST_NODE *parentASTNode)
{

    if(parentASTNode == NULL)
    {
        //
    } else
        {
            parentASTNode->symbolTable = symbolNode;
            AST_NODE *funcDef = parentASTNode;
            //Find where the function is being used
            //Not the best way to do this, but keeps it from skipping any values
            if(funcDef->data.function.oper != CUSTOM_OPER && strcmp(funcDef->data.function.ident, symbolNode->ident) != 0)
            {
                funcDef = funcDef->data.function.opList;
                while (funcDef->data.function.oper != CUSTOM_OPER && strcmp(funcDef->data.function.ident, symbolNode->ident) !=0)
                {
                    funcDef = funcDef->data.function.opList->next;
                }
            }

            if(parentASTNode->symbolTable->stack != NULL)
            {
                //Assign to stack to eval custom function's symbol stack's values later
                STACK_NODE *curStackNode = parentASTNode->symbolTable->stack;
                AST_NODE *curOP = funcDef->data.function.opList;
                while(curStackNode != NULL)
                {
                    curStackNode->val = curOP;

                    curStackNode = curStackNode->next;
                    curOP = curOP->next;
                }
            }
        }

    return parentASTNode;
}

// Called when an f_expr is created (see ciLisp.y).
// Creates an AST_NODE for a function call.
// Sets the created AST_NODE's type to function.
// Populates the contained FUNC_AST_NODE with:
//      - An OPER_TYPE (the enum identifying the specific function being called)
//      - 2 AST_NODEs, the operands
// SEE: AST_NODE, FUNC_AST_NODE, AST_NODE_TYPE.
AST_NODE *createFunctionNode(char *funcName, AST_NODE *opList)
{

    AST_NODE *node;
    size_t nodeSize;

    // allocate space (or error)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    // TODO set the AST_NODE's type, populate contained FUNC_AST_NODE
    //for now just free the name until later2
    // NOTE: you do not need to populate the "ident" field unless the function is type CUSTOM_OPER.
    // When you do have a CUSTOM_OPER, you do NOT need to allocate and strcpy here.
    // The funcName will be a string identifier for which space should be allocated in the tokenizer.
    // For CUSTOM_OPER functions, you should simply assign the "ident" pointer to the passed in funcName.
    // For functions other than CUSTOM_OPER, you should free the funcName after you're assigned the OPER_TYPE.
    node->type = FUNC_NODE_TYPE;
    node->data.function.oper = resolveFunc(funcName);
    node->data.function.opList = opList;
    //Populate operations for the function node using the list
    AST_NODE *curOpNode;
    curOpNode = opList;
    while (curOpNode != NULL)
    {
        curOpNode->parent = node;
        curOpNode = curOpNode->next;
    }

    // TODO Set Parents
    // TODO add the custom functions name to the node
    node->data.function.ident = malloc(sizeof(funcName)+1);
    strcpy(node->data.function.ident, funcName);
    if(node->data.function.oper != CUSTOM_OPER)
        free(funcName);//Check if a custom function BEFORE FREEING!

    return node;
}

AST_NODE *addFunctionNodeToList(AST_NODE *nextOp, AST_NODE *curOpList)
{

    if(nextOp == NULL)
    {
        //No Parent
    } else
    {
        nextOp->next = curOpList;
    }

    return nextOp;
}

//Make a conditional node
AST_NODE *createConditionalNode(AST_NODE *condNode, AST_NODE *trueNode, AST_NODE *falseNode)
{
    AST_NODE *node;
    size_t nodeSize;

    // allocate space (or error)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    node->type = COND_NODE_TYPE;
    node->data.condition.cond = condNode;
    node->data.condition.trueCond = trueNode;
    node->data.condition.falseCond = falseNode;

    return node;
}

// Called after execution is done on the base of the tree.
// (see the program production in ciLisp.y)
// Recursively frees the whole abstract syntax tree.
// You'll need to update and expand freeNode as the project develops.
void freeNode(AST_NODE *node)
{
    if (!node)
        return;

    if (node->type == FUNC_NODE_TYPE)
    {
        // Recursive calls to free child nodes
        freeNode(node->data.function.opList->next);


        // Free up identifier string if necessary
        if (node->data.function.oper == CUSTOM_OPER)
        {
            free(node->data.function.ident);
        }
    }

    free(node);
}

// Evaluates an AST_NODE.
// returns a RET_VAL storing the the resulting value and type.
// You'll need to update and expand eval (and the more specific eval functions below)
// as the project develops.
RET_VAL eval(AST_NODE *node)
{
    if (!node)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN}; // see NUM_AST_NODE, because RET_VAL is just an alternative name for it.

    // TODO complete the switch.
    // Make calls to other eval functions based on node type.
    // Use the results of those calls to populate result.
    // in the long run you may want to change &node->data.function to just generic node
    switch (node->type)
    {
        case FUNC_NODE_TYPE:
            result = evalFuncNode( &node->data.function);
            break;
        case NUM_NODE_TYPE:
            result = evalNumNode( &node->data.number);
            break;
        case SYMBOL_NODE_TYPE:
            result = evalSymbolNode(node);
            break;
        case COND_NODE_TYPE:
            result = evalConditionalNode(node);
            break;
        default:
            yyerror("Invalid AST_NODE_TYPE, probably invalid writes somewhere!");
    }

    return result;
}  

// returns a pointer to the NUM_AST_NODE (aka RET_VAL) referenced by node.
// DOES NOT allocate space for a new RET_VAL.
RET_VAL evalNumNode(NUM_AST_NODE *numNode)
{
    //default this will return not a number
    if (!numNode)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};

    // TODO populate result with the values stored in the node.
    // SEE: AST_NODE, AST_NODE_TYPE, NUM_AST_NODE
    result.type = numNode->type;
    result.value = numNode->value;

    return result;
}


RET_VAL evalFuncNode(FUNC_AST_NODE *funcNode)
{
    if (!funcNode)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};

    // TODO populate result with the result of running the function on its operands.
    // SEE: AST_NODE, AST_NODE_TYPE, FUNC_AST_NODE
    //eval these in the switch
    // return double if one type is a double
    //recommend look at op in h file and get one op working to start
    //op2 only for operations that take two ops that aren't add, mult, or print
    AST_NODE *curOpNode;
    curOpNode = funcNode->opList;
    RET_VAL op1;
    RET_VAL op2;
    //Used to find the user's custom function
    AST_NODE *curSymbol = NULL;

    //Use for print op to make sure it prints correctly with a read func
    bool printFlag = false;

    if(funcNode->oper != READ_OPER && funcNode->oper != RAND_OPER)
        funcNode->oper = checkFunctionOpList(funcNode);
    switch (funcNode->oper)
    {
        case NEG_OPER:
            op1 = eval(curOpNode);
            result.value = op1.value;
            result.type = op1.type;
            break;
        case ABS_OPER:
            op1 = eval(curOpNode);
            result.value = fabs(op1.value);
            result.type = op1.type;
            break;
        case EXP_OPER:
            op1 = eval(curOpNode);
            result.value = exp(op1.value);
            result.type = op1.type;
            break;
        case SQRT_OPER:
            op1 = eval(curOpNode);
            result.value = sqrt(op1.value);
            result.type = op1.type;
            break;
        case ADD_OPER:
            result.value = 0;
            while (curOpNode != NULL)
            {
                op1 = eval(curOpNode);
                result.value += op1.value;
                result.type = result.type || op1.type;
                curOpNode = curOpNode->next;
            }
            break;
        case SUB_OPER:
            op1 = eval(curOpNode);
            op2 = eval(curOpNode->next);
            result.value = op1.value - op2.value;
            result.type = op1.type || op2.type;
            break;
        case MULT_OPER:
            result.value = 1;
            while (curOpNode != NULL)
            {
                op1 = eval(curOpNode);
                result.value *= op1.value;
                result.type = result.type || op1.type;
                curOpNode = curOpNode->next;
            }
            break;
        case DIV_OPER:
            op1 = eval(curOpNode);
            op2 = eval(curOpNode->next);
            result.value = op1.value / op2.value;
            result.type = op1.type || op2.type;
            break;
        case REMAINDER_OPER:
            op1 = eval(curOpNode);
            op2 = eval(curOpNode->next);
            result.value = remainder(op1.value, op2.value);
            result.type = op1.type || op2.type;
            break;
        case LOG_OPER:
            op1 = eval(curOpNode);
            result.value = log(op1.value);
            result.type = op1.type;
            break;
        case POW_OPER:
            op1 = eval(curOpNode);
            op2 = eval(curOpNode->next);
            result.value = pow(op1.value, op2.value);
            result.type = op1.type || op2.type;
            break;
        case MAX_OPER:
            op1 = eval(curOpNode);
            op2 = eval(curOpNode->next);
            result.value = fmax(op1.value, op2.value);
            result.type = op1.type || op2.type;
            break;
        case MIN_OPER:
            op1 = eval(curOpNode);
            op2 = eval(curOpNode->next);
            result.value = fmin(op1.value, op2.value);
            result.type = op1.type || op2.type;
            break;
        case EXP2_OPER:
            op1 = eval(curOpNode);
            result.value = exp2(op1.value);
            result.type = op1.type;
            break;
        case CBRT_OPER:
            op1 = eval(curOpNode);
            result.value = cbrt(op1.value);
            result.type = op1.type;
            break;
        case HYPOT_OPER:
            op1 = eval(curOpNode);
            op2 = eval(curOpNode->next);
            result.value = hypot(op1.value, op2.value);
            result.type = op1.type || op2.type;
            break;
        case PRINT_OPER:
            while (curOpNode != NULL)
            {
                op1 = eval(curOpNode);
                result.value = op1.value;
                result.type = op1.type;
                if(!printFlag)
                {
                    printFlag = true;
                    printf("\n=>");
                }
                printf(" %lf", result.value);
                curOpNode = curOpNode->next;
            }
            printf("\n%lf", result.value);
            break;
        case READ_OPER:
            result = checkReadNumber();
            break;
        case RAND_OPER:
            result.value = rand()/(double)RAND_MAX;
            result.type = DOUBLE_TYPE;
            break;
        case EQUAL_OPER:
            op1 = eval(curOpNode);
            op2 = eval(curOpNode->next);
            if(op1.value == op2.value)
                result.value = 1;
            else
                result.value = 0;
            break;
        case LESS_OPER:
            op1 = eval(curOpNode);
            op2 = eval(curOpNode->next);
            if(op1.value < op2.value)
                result.value = 1;
            else
                result.value = 0;
            break;
        case GREATER_OPER:
            op1 = eval(curOpNode);
            op2 = eval(curOpNode->next);
            if(op1.value > op2.value)
                result.value = 1;
            else
                result.value = 0;
            break;
        case CUSTOM_OPER:
            //Find the user's program
            curSymbol = funcNode->opList->parent;
            while (curSymbol->symbolTable == NULL)
            {
                curSymbol = curSymbol->parent;
            }
            //Parent the written function symbols to the function call so we can use the stack nodes
            //funcNode->opList->parent->symbolTable->val->parent = funcNode->opList->parent;
            //op1 = eval(funcNode->opList->parent->symbolTable->val);
            curSymbol->symbolTable->val->parent = funcNode->opList->parent;
            op1 = eval(curSymbol->symbolTable->val);
            result.value = op1.value;
            result.type = op1.type || curSymbol->symbolTable->val_type;
            break;
        default:
            printf("\nNot a valid operation!\n");
            break;
    }

    return result;
}

RET_VAL evalSymbolNode( AST_NODE *symbolNode)
{
    if (!symbolNode)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};

    AST_NODE *symbolTableNode;
    //symbolTable = symbolNode->parent->symbolTable;
    symbolTableNode = symbolNode->parent;

    //Find symbol table
    while (symbolTableNode->symbolTable == NULL)
    {
        symbolTableNode = symbolTableNode->parent;
    }

    SYMBOL_TABLE_NODE *curNode;
    curNode = symbolTableNode->symbolTable;

    //Check if user made function, in that case we use the stack variables instead
    if(curNode->type == LAMBDA_TYPE)
    {
        AST_NODE *curStackFrame = curNode->stack->val;
        result = eval(curStackFrame);
        //Once done eval user made function symbols the value gets popped off the stack
        curNode->stack = curNode->stack->next;
        return result;
    } else
        {
            //Find the Symbol if not user made function
            while(curNode != NULL)
            {
                if(strcmp(curNode->ident, symbolNode->data.symbol.ident) == 0)
                {
                    result = eval(curNode->val);
                    return result;
                }

                curNode = curNode->next;
            }
        }

    printf("\nRuntime Error: Missing Symbol!");
    yyerror("Missing Symbol!");
    return result;
}

RET_VAL evalConditionalNode(AST_NODE *condNode)
{
    if (!condNode)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};

    //Make sure the conditional has the need symbols in scope
    // for each of the cases.
    condNode->data.condition.cond->symbolTable = condNode->symbolTable;
    condNode->data.condition.trueCond->symbolTable = condNode->symbolTable;
    condNode->data.condition.falseCond->symbolTable = condNode->symbolTable;

    result = eval(condNode->data.condition.cond);

    if(result.value != 0)
    {
        result = eval(condNode->data.condition.trueCond);
    }
    else
        {
            result = eval(condNode->data.condition.falseCond);
        }

    return result;

}

// prints the type and value of a RET_VAL
void printRetVal(RET_VAL val)
{
    // TODO print the type and value of the value passed in.
    printf("\n%s value: %f", nodeNames[val.type], val.value);
}

//-------HELPER FUNCTION SECTION-------

bool checkNumberType(double val)
{
    double truncVal = (int)val;
    if(truncVal == val)
        return true;
    else
        return false;
}

RET_VAL checkReadNumber()
{
    RET_VAL result = {INT_TYPE, NAN};

    //Read into a buffer in order to convert into decimal and take care
    //of edge cases.
    char inputBuffer[256];
    char *currentRead;
    printf("read := ");
    if(fgets(inputBuffer, sizeof(inputBuffer), stdin))
    {
        int length = strlen(inputBuffer);
        inputBuffer[length-1] = '\0';

        if(inputBuffer[0] == '.')
        {
            inputBuffer[0] = '0';
            inputBuffer[1] = '\0';
        }
        else
            {
                //Find a lone dot and get ready to convert our value to return
                int i = 1;
                while (inputBuffer[i] != '\0')
                {
                    if(inputBuffer[i] == '.')
                    {
                        result.type = DOUBLE_TYPE;
                        if(inputBuffer[i+1] == '\0')
                        {
                            inputBuffer[i + 1] = '0';
                            inputBuffer[i + 2] = '\0';
                            break;
                        }
                    }
                    i++;
                }
            }
    }
    //Exit input checking and convert values
    result.value = strtod(inputBuffer, &currentRead);
    return result;
}

NUM_TYPE evalType(char *type)
{
    NUM_TYPE itemType;

    if(strcmp("int", type) == 0)
    {
        itemType = INT_TYPE;
    }
    else
    {
        itemType = DOUBLE_TYPE;
    }

    return  itemType;
}

int checkFunctionOpList(FUNC_AST_NODE *funcNode)
{
    AST_NODE *curOpNode;
    curOpNode = funcNode->opList;

    if(curOpNode == NULL)
    {
        printf("\nERROR: too few parameters for the function %s\n", funcNode->ident);
        return -1;
    }

    if(funcNode->oper < 4 || funcNode->oper == 9 || funcNode->oper == 13 || funcNode->oper == 14)
    {
        return funcNode->oper;
    }

    if(funcNode->oper != ADD_OPER && funcNode->oper != MULT_OPER && funcNode->oper != PRINT_OPER)
    {
        if(curOpNode->next == NULL)
        {
            printf("\nERROR: too few parameters for the function %s\n", funcNode->ident);
            return -1;
        }
        if(curOpNode->next->next != NULL )
            printf("\nWARNING: too many parameters for the function %s\n", funcNode->ident);
    }

    return funcNode->oper;
}
