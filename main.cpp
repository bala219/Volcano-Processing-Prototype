#include <string>
#include <core/global_definitions.hpp>
#include <core/base_column.hpp>
#include <core/column_base_typed.hpp>
#include <core/column.hpp>
#include <core/compressed_column.hpp>

/*
 * Include the query files here
 */
#include "queries/q1.h"
#include "queries/q2.h"

using namespace CoGaDB;

int main(){

    //Call to the unit test function for processing
    executeQ2();

 return 0;
}


