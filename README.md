# CI_LISP

Darrell Wulff
CI LISP Project/Lab
Fall 2019

# CURRENT SUBMISSION: TASK 7

* Task 1
* Task 2
* Task 3
* Task 4
* Task 5
* Task 6
* Task 7 -

    **Changes** Task 7 saw the introduction of user defined functions
    to the grammar using the lambda expression. Here a function name
    , formal parameters, and function body are defined in order for
    custom functionality.
    
    I expanded the yacc file to use createSymbolTableNode() more, but with
    different options of parameters that would go under a multitude of checking
    to find the correct values.
    
    createArgNode() is added to create and link argument stacks together.
    These argument stacks vals are then filled up by the correct operations
    when the symbolTable node gets parented to it's astNode. 
    
    evalSymbolNode now searches for the function stack after finding the symbols
    to be used with the stack. It calls eval() on each stack node and pops of the
    old one once it returns to the eval() call in evalSymbolNode.
    
    Issues: Some times the error checking would lead to a segment fault as things would
    be Null and were try to access them.
    
    Sample Runs Located in Task7TestOutputs
     
* Task 8

    **Not Able To Complete**
