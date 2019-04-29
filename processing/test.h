//
// Created by gurumurt on 4/12/19.
//

#ifndef DB2_PROGRAMMING_PROJECT_TEST_H
#define DB2_PROGRAMMING_PROJECT_TEST_H

#endif //DB2_PROGRAMMING_PROJECT_TEST_H

namespace CoGaDB{

#define EOL    -2147483648

    class volcanoOperator{

    public:

        //member variables
        volcanoOperator* child;

        volcanoOperator(){

            this->child=NULL;
        }
        //member functions
        virtual void open()=0;
        virtual void next()=0;
        virtual void close()=0;
        ~volcanoOperator()=0;
    };


    class scan:public volcanoOperator{

        size_t i;
        boost::shared_ptr<Column<int> > column_ptr;
    public:

        scan(boost::shared_ptr<Column<int> > ptr){
            this->column_ptr = ptr;
            this->child=NULL;
            i=0;
        }

        void open(){


            std::cout<<"All operations opened"<<std::endl;
        }

        int next(){

            if(column_ptr->size()>i){
                return boost::any_cast<int>(column_ptr->get(i++));
            }

            return EOL;
        }

        void close(){

        }
    };

    class selection:public volcanoOperator{

    public:

        int SelectionOperand;

        void open(int selectionOp = 100){

            this->SelectionOperand = selectionOp;
            this->child->open();
        }

        int next(){

            int data;

            do{
                data = this->child->next();
            }while(data>this->SelectionOperand&&data!=EOL);

            return data;
        }

        void close(){

        }
    };

    class reduce:public volcanoOperator{

    public:
        int sum;
        void open(){

            sum = 0;
            this->child->open();

            int data=0;

            while(data !=EOL){
                sum +=data;
                data = child->next();
            }
        }

        int next(){

            return sum;
        }

        void close(){

        }
    };

    class print: public volcanoOperator{

    public:
        void open(){

            child->open();
        }

        int next(){

            int val = child->next();
            std::cout<<val<<std::endl;
            return val;
        }

        void close(){
        }

    };
}