# CI_LISP

Darrell Wulff
CI LISP Project/Lab
Fall 2019

# CURRENT SUBMISSION: TASK 5

* Task 1   
* Task 2 
* Task 3
* Task 4
* Task 5 -

    **Changes** Task 5 had s-expressions expanded into s-expression lists.
    These lists allowed for the functions add, mult, and print to be
    expanded beyond 2 operations.
    
    I implemented while loops inside the ADD, MULT, and PRINT operations
    when the are reached in eval. This allows the program to keep accumulating
    nodes to be used in the result of these oprerations.
    
    I expanded the definitions of f-expr and s-expr to allow for funcs to pass in
    the the list of operations. This will happen for all functions, but most only
    use 1 or two.
    
    checkFuncOpList() was added as a helper function to conduct a series of checks
    in order for the function operator to be the correct one. It also helps issue
    out warnings for the functions.
    
    Issues: Sometimes when a function that takes two operations will not be able to read
    in a dot character if you input one large double ex (sub 4532.34)
    
    Sample Runs: Located in Task5Outputs file
    
* Task 6
* Task 7
* Task 8
