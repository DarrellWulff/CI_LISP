# CI_LISP

Darrell Wulff
CI LISP Project/Lab
Fall 2019

# CURRENT SUBMISSION: TASK 3

* Task 1 -
    
    **Changes:** Task 1 implemented each of the comments 
    
      createNumberNode: now assigns the type of number node and the value.
      
      createFuntionNode: populates the function node 
      and find the name with resolve function.
      
      eval: switch made based on language grammar.
      
      evalNumNdoe: populates number node.
      
      evalFuncNode: switch contains the correct operations neg through hyp 
      and populates the result node.
      
      printRetVal: prints correct return value.
      
      added checkNumberType helper function.
      
      Issues: Only main issue I found is that when printing the value that
      is supposed to be an integer, the value is followed by multiple zeros. This is
      happening since I changed it to just a double value.
      
      Sample Runs: My task 1 runs are contained in the "Task1TestOuputs" file. The lexer takes in the
      input then bison matches it to the correct expersion passesing the th $values. 
      These values are lined up to the correct ast_node types to complete the operations.
    
* Task 2 -

	**Changes** Task 2 implemented the let sections from the updated grammar

	Created Symbol_AST_Node struct to hold symbol identifications.
	
	Created Symbol Table Node to implement as a linked list of potential symbols to matched
	when compiling CILISP code.
	
	createSymbolNode: allows for creation of a symbol id to then be refernced from a parent
	AST_NODE in the let section

	createSymbolTableNode: Allows for a char* id to store a lec section expression's eventual 
	value.

	addSymbolToList: This will add a symbol AST node to the current linked list. I implemented
	it to just make the newest node to become the head some then when searching for the
	the correct symbol it can go the the newest scope.

	parentToAstNode: allows for a symbol table list head to be referenced by an AST node parent
	in the lec section.
	
	Expanded lexx and yacc files to parse for let sections as the lexer will look for symbols to
	feed to yacc in order to construct the sections.

	Issues: After inputing around more than 8-10 let sections, the next input appears to be stuck
	inside an infinite loop. Also error message is tied to debug prints when missing symbols
	and just shows number node = nan when there is no debug prints.

	Sample Runs: Task 2 runs are contained in task2 run files in the task2 branch of the 
	repository. One shows all let sections on site running and the other shows 
	the missing symbol error debug print message.


* Task 3 -
    
    **Changes** Task 3 introduced typing to the grammar allowing for an optional
    type casting INT or DOUBLE onto s-expressions
    
    Started with the lexx and yacc files to enable optional type tokens to be 
    read into createNumNode().
    
    I updated the symbol table node structure to accommodate for different types,
    and implemented a series of checks in order to cast properly before and after
    a calculation. If there is a type desired at runtime the yacc file will
    pass it into the creation of the symbol or number.
    
    Issues: Not sure if the final value node it prints the value out from is the correct node.
    That seems to be the only issue here.
    
    Sample Runs: Are located in the Task3TestOutputs file.
    
* Task 4
* Task 5
* Task 6
* Task 7
* Task 8
