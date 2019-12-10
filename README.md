# CI_LISP

Darrell Wulff
CI LISP Project/Lab
Fall 2019

# CURRENT SUBMISSION: TASK 6

* Task 1
* Task 2
* Task 3
* Task 4
* Task 5
* Task 6 - 

    **Changes** Task 6 allowed for greater functionality through the new
    piece of grammar which were conditionals. cond tokens in the yacc file
    were piece together with different parts that function much like a
    if statement. The conditional's first clause is the statement to be
    evaluated and other clauses are what occurs if it is true or false.
    
    createConditonalNode() takes in the 3 s-expressions from the yacc file
    and inserts them to be used later onto one node.
    
    The functions Less Greater and Equal were implemented as if statements
    that took operation's results to calculate the logic.
    
    Rand used casting of randmax in order to return a correct random range.
    
    Read use a helper function checkReadNumber to use a buffer to parse input
    and catch any dot(.) characters that would change the number into a double
    type automatically. Max read size was set to 256, so any number longer than
    that might be considered an issue.
    
    Issues: Sometimes a let section will cause a segmentation fault when running, but 
    it does not occur in debug mode. It appears there was an issue with the lex and 
    yacc file picking up the different tokens needed which is probably part of the issue.
    
    Sample Runs: Located in Task6TestOutputs file.
* Task 7
* Task 8
