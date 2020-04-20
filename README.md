# Volcano-Processing-Prototype

RDBMS engine is equipped with versatile and flexible operations for processing data. A user given SQL statement is split into multiple granular operations. These operations then work together in producing output.

## Processing Model

Based on the processing engine the data are processed differently. In an abstract view, there are two different processing models

1.  Iterator-based
2. Compiled

In compiled execution, the SQL is parsed and compiled into a tightly wounded function and the data are executed. Though this method is highly efficient, it also requires complex re-engineering of operations to be adapted for execution. Hence, a more flexible method - volcano model is used.

## Volcano Model

Also known as iterator-based processing, the input is iteratively processed by the operator sequence mentioned in the SQL. This method was devised by Dr.Goetz Gräfe and is detailed in the paper

[Query evaluation techniques for large databases](https://web.stanford.edu/class/cs346/2014/graefe.pdf) - Table 1. Examples of Iterator Functions shows all the necessary steps for implementing the open, next and close functions

The operations in this technique, iteratively consume data from previous operation, process input data and pass it to the next operator. This provides the flexibility of processing any arbitrary functions without any rework.

### Driver
The flexibility of the process model is mainly due to the driver structure of the volcano operation. Any operation to be included in the model has to have implement the driver structure given below.

```C
open(){

  //All initialization instructions come here
}

next(){
  
  //All data processing instructions come here
  //This function must always forward result or EOL
}

close(){
  //Final bookkeeping and garbage collection instructions come here
}
```

## Prototype
This repository holds the bare-metal processing framework required for processing a given SQL statement. *processing/volcano.hpp* contains the skeleton for each of the operations that are used to evaluate a given SQL.

# Task

Last ATDB proect was to have a hands-on in understanding the processing model in general. This time we move a step further by directly incorporating functionalities of modern processors for faster processing. 

Your task is to implement the following

1. Implement missing operations in *volcano.hpp* using SIMD (Selection, project, sort, hash joins, aggregation)
2. Implement the volcano based execution plan as well as compiled execution plan for the given queries (Note: the compiled query plan musz also be in SIMD)

The results of both the models must evaluate to equal.

# Optional Tasks
3. Use threading for parallel execution of multiple operations running in parallel. Check the exchange operator for more details about this [Encapsulation of parallelism in the volcano processing model](https://w6113.github.io/files/papers/volcanoparallelism-89.pdf) 

# Details

1. The Query 3 in the comment misses "GROUP BY A"
