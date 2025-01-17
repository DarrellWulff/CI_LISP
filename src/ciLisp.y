%{
    #include "ciLisp.h"
%}

%union {
    double dval;
    char *sval;
    struct ast_node *astNode;
    struct symbol_table_node *symNode;
}

%token <sval> FUNC SYMBOL TYPE
%token <dval> INT DOUBLE
%token LPAREN RPAREN LET COND LAMBDA EOL QUIT

%type <astNode> s_expr f_expr s_expr_list number
%type <symNode> let_section let_list let_elem arg_list

%%

program:
    s_expr EOL {
        fprintf(stderr, "yacc: program ::= s_expr EOL\n");
        if ($1) {
            printRetVal(eval($1));
            freeNode($1);
        }
    };



s_expr:
    number {
        fprintf(stderr, "yacc: s_expr ::= number\n");
        $$ = $1;
    }
    | SYMBOL {
        fprintf(stderr, "yacc: symbol ::= SYMBOL\n");
        $$ = createSymbolNode($1);
    }
    | f_expr {
    	fprintf(stderr, "yacc: s_expr ::= f_expr\n");
        $$ = $1;
    }
    | QUIT {
        fprintf(stderr, "yacc: s_expr ::= QUIT\n");
        exit(EXIT_SUCCESS);
    }
    | LPAREN let_section s_expr RPAREN {
        fprintf(stderr, "yacc: s_expr ::= LPAREN let_section s_expr RPAREN\n");
        $$ = parentToAstNode($2, $3);
    }
    | LPAREN COND s_expr s_expr s_expr RPAREN {
    	fprintf(stderr, "yacc: s_expr ::= LPAREN COND s_expr s_expr s_expr RPAREN\n");
    	$$ = createConditionalNode($3, $4, $5);
    }
    | error {
        fprintf(stderr, "yacc: s_expr ::= error\n");
        yyerror("unexpected token");
        $$ = NULL;
    };

number:
    INT {
        fprintf(stderr, "yacc: number ::= INT\n");
        $$ = createNumberNode(NULL, $1);
    }
    | DOUBLE {
        fprintf(stderr, "yacc: number ::= DOUBLE\n");
        $$ = createNumberNode(NULL, $1);
    }
    | TYPE INT {
        fprintf(stderr, "yacc: number ::= INT\n");
        $$ = createNumberNode($1, $2);
    }
    | TYPE DOUBLE {
        fprintf(stderr, "yacc: number ::= DOUBLE\n");
        $$ = createNumberNode($1, $2);
    };

f_expr:
    LPAREN FUNC s_expr_list RPAREN {
        fprintf(stderr, "yacc: s_expr ::= LPAREN FUNC s_expr_list RPAREN\n");
        $$ = createFunctionNode($2, $3);
    }
    | LPAREN SYMBOL s_expr_list RPAREN {
    	fprintf(stderr, "yacc: s_expr ::= LPAREN SYMBOL s_expr_list RPAREN\n");
    	$$ = createFunctionNode($2, $3);
    };

s_expr_list:
    s_expr {
    	fprintf(stderr, "yacc: program ::= s_expr\n");
    	$$ = $1;
    }
    | s_expr s_expr_list{
    	fprintf(stderr, "yacc: program ::= s_expr s_expr_list\n");
	$$ = addFunctionNodeToList($1, $2);
    };

let_section:
    LPAREN LET let_list RPAREN{
    	fprintf(stderr,"yacc: LET_SECTION\n");
	$$ = $3;
    };

let_list:
    let_elem{
    	fprintf(stderr,"yacc: LET_ELEM\n");
	$$ = $1;
    }
    | let_list let_elem{
    	fprintf(stderr,"yacc: LET_LIST LET_ELEM\n");
	$$ = addSymbolToList($1, $2);
    };

let_elem:
    LPAREN SYMBOL s_expr RPAREN{
    	fprintf(stderr,"yacc:  LPAREN SYMBOL s_expr RPAREN\n");
	$$ = createSymbolTableNode($2, $3, NULL, NULL);
    }
    | LPAREN TYPE SYMBOL s_expr RPAREN{
        fprintf(stderr,"yacc:  LPAREN SYMBOL s_expr RPAREN\n");
    	$$ = createSymbolTableNode($3, $4, $2, NULL);
    }
    | LPAREN SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN{
        fprintf(stderr,"yacc:  LPAREN TYPE SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN\n");
        $$ = createSymbolTableNode($2, $7, NULL, $5);
    }
    | LPAREN TYPE SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN{
    	fprintf(stderr,"yacc:  LPAREN TYPE SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN\n");
    	$$ = createSymbolTableNode($3, $8, $2, $6);
    };

arg_list:
    SYMBOL arg_list{
	$$ = createSymbolArgNode($1, $2);
    }
    | SYMBOL{
    	fprintf(stderr, "yacc: symbol ::= SYMBOL\n");
	$$ = createSymbolArgNode($1, NULL);
    };

%%

