//
// Created by gurumurt on 4/12/19.
//

#ifndef DB2_PROGRAMMING_PROJECT_Q1_H
#define DB2_PROGRAMMING_PROJECT_Q1_H

#endif //DB2_PROGRAMMING_PROJECT_Q1_H

#include <string>
#include <core/global_definitions.hpp>
#include <core/base_column.hpp>
#include <core/column_base_typed.hpp>
#include <core/column.hpp>
#include <core/compressed_column.hpp>
#include <processing/volcano.hpp>

#include "../unittest.hpp"
/*
 * SELECT SUM(A)
 * FROM Table1
 * WHERE A<10;
 */

namespace CoGaDB{

    int compiledQ1(boost::shared_ptr<Column<int> > column_ptr){

        int summation=0;
        for(int i=0;i<column_ptr->size();i++){

            int val = boost::any_cast<int>(column_ptr->get(i));
            if(val<50)
                summation+=val;
        }

        return summation;
    }

    void executeQ1(){

        boost::shared_ptr<Column<int> > column_ptr (new Column<int>("int column",INT));
        std::vector<int> reference_data(100);
        fill_column<int>(column_ptr, reference_data);

        scan  s(column_ptr);
        selection sel(&s);
        reduce r(&sel);

        r.open();
        int data = r.next();
        int summa;
        do{
            summa = data;
            data = sel.next(); //summa used as the last value of data becomes EOL
        }while(data!=EOL);
        r.close();

        int summation = compiledQ1(column_ptr);

        std::cout<<std::memcmp(&summation, &summa,2)<<std::endl;
    }

}
