/*! Base class for operations using Volcano model
 * Each operator has three basic functions and uses them for execution by calling next()
 */

#pragma once

#include <core/column.hpp>
#include <string>
#include <core/global_definitions.hpp>
#include <core/base_column.hpp>
#include <core/column_base_typed.hpp>
#include <core/column.hpp>
#include <core/compressed_column.hpp>


namespace CoGaDB {

boost::shared_ptr<Column<int> > column_ptr (new Column<int>("int column",INT));
#define EOL    -2147483648


    int* i;
    void scan_open(){

        i = (int*)calloc(1, sizeof(int));
        std::cout<<"open done"<<std::endl;
    }

    int scan_next(){

        if(++*i<column_ptr->size())
                return boost::any_cast<int>(column_ptr->get(*i));
        else
            return EOL;
    }

    void scan_close(){
        delete i;
        std::cout<<"close done"<<std::endl;
    }

    void selection_open() {
        scan_open();
    }

	int selection_next() {
        int data;
        do{
            data = scan_next();

        }while(data<10&&data!=EOL);

        return data;
	}

	void selection_close() {
        scan_close();
	}

	int *sum;
    int data = 0;
    void reduce_open() {

		sum = (int *) calloc(1, sizeof(int));
        selection_open();
	}

	int reduce_next() {

        int data = selection_next();

        if(data==EOL)
            *sum=EOL;

        while(data != EOL){
            *sum += data;
            data = selection_next();
        }

       return *sum;
    }

	void reduce_close() {

		delete sum;
        selection_close();
	}

    int* val;
    void print_open() {

        val = (int*)calloc(1,sizeof(int));
		reduce_open();

	}

	int print_next() {

        *val = reduce_next();
		std::cout << *val << std::endl;
		return *val;
	}

	void print_close() {

        delete val;
		reduce_close();
	}

    void iterate() {

 		print_open();
		int data = 0;
		while(data!= EOL){

			data = print_next();
		}
		print_close();

	}

}