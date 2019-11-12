# CI_LISP

Darrell Wulff
CI LISP Project/Lab
Fall 2019

# CURRENT SUBMISSION: TASK 1

* Task 1 -
    
    **Changes:** Task 1 implemented each of the comments 
    
      createNumberNode: now assigns the type of number node and the value.
      
      createFuntionNode: populates the function node and find the name with resolve function
      
      eval: switch made based on language grammar
      
      evalNumNdoe: populates number node
      
      evalFuncNode: switch contains the correct operations neg through hyp and populates the result node
      
      printRetVal: prints correct return value
      
      added checkNumberType helper function
      
      Issues: Only main issue I found is that when printing the value that
      is supposed to be an integer, the value is followed by multiple zeros. This is
      happening since I changed it to just a double value.
      
      Sample Runs: My task 1 runs are contained in the "Task1Ouputs" file. The lexer takes in the
      input then bison matches it to the correct expersion passesing the th $values. These values are lined 
      up to the correct ast_node types to complete the operations.
    
* Task 2
* Task 3
* Task 4
* Task 5
* Task 6
* Task 7
* Task 8
