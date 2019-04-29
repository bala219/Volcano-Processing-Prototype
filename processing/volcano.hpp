#pragma once

#include <string>
#include <core/global_definitions.hpp>
#include <core/base_column.hpp>
#include <core/column_base_typed.hpp>
#include <core/column.hpp>
#include <core/compressed_column.hpp>

#include "operator.hpp"

namespace CoGaDB{



    class volcanoOperator{

    public:
        volcanoOperator* child;
        volcanoOperator(){
            this->child=NULL;
        }
        virtual void open()=0;
        virtual int next()=0;
        virtual void close()=0;
        ~volcanoOperator(){};

    };

    class scan: public volcanoOperator{

        size_t i;
        boost::shared_ptr<Column<int> > column_ptr;

    public:

        scan(){}

        scan(boost::shared_ptr<Column<int> > ptr){

            this->column_ptr = ptr;
        }

        void open(){

            i=0;
            std::cout<<"open Column pointer"<<std::endl;
            std::cout<<"Column Size: "<<column_ptr->size()<<std::endl;
        };

        int next(){

            if(column_ptr->size() > i)

                return boost::any_cast<int>(column_ptr->get(i++));
            return EOL;
        };

        void close(){

            column_ptr.reset();
            i=0;
        }
    };

    class selection: public volcanoOperator{

        int operand;

    public:

        selection(){}
        selection(volcanoOperator* child,int oper = 50){

            this->child = child;
            operand = oper;
        }

        void open(){

            this->child->open();
        }

        int next(){
            int data = child->next();
            while(data !=EOL){
                if(data < operand)
                    return data;
                data = child->next();
            }
            return EOL;
        }
        void close(){}
    };

    class reduce: public volcanoOperator{

        int sum;
        bool flag;

    public:

        reduce(){}
        reduce(volcanoOperator* child){

            flag=false;
            this->child = child;
            sum = 0;
        }

        void open(){

            this->child->open();

            int data = child->next();
            while(data !=EOL){
                sum+=data;
                data = child->next();
            }
        }

        int next(){

            if(!flag){
                flag=true;
                return sum;
            }

            return EOL;

        }
        void close(){
        }
    };

    class Join: public volcanoOperator{

    public:

        Join(){}
        Join(volcanoOperator* child){

            this->child = child;
        }

        void open(){

        }

        int next(){

        }

        void close(){
        }
    };

    class GroupedAggregation: public volcanoOperator{

    public:

        GroupedAggregation(){}
        GroupedAggregation(volcanoOperator* child){

            this->child = child;
        }

        void open(){

        }

        int next(){

        }

        void close(){
        }
    };

    class sorting: public volcanoOperator{

    public:

        sorting(){}
        sorting(volcanoOperator* child){

            this->child = child;
        }

        void open(){

        }

        int next(){

        }

        void close(){
        }
    };
}