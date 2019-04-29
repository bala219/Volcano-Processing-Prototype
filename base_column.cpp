
#include <core/base_column.hpp>
#include <core/column.hpp>
#include <iostream>

using namespace std;

namespace CoGaDB{

	ColumnBase::ColumnBase(const std::string& name, AttributeType db_type) : name_(name), db_type_(db_type){

	}

	ColumnBase::~ColumnBase(){}

	AttributeType ColumnBase::getType() const throw(){
		return db_type_;
	}

	const string ColumnBase::getName() const throw(){
		return name_;
	}

	const ColumnPtr createColumn(AttributeType type, const std::string& name){

	ColumnPtr ptr;
	if(type==INT){
		ptr=ColumnPtr(new Column<int>(name,INT));
	}else if(type==FLOAT){
		ptr=ColumnPtr(new Column<float>(name,FLOAT));
	}else if(type==VARCHAR){
		ptr=ColumnPtr(new Column<string>(name,VARCHAR));
	}else if(type==BOOLEAN){
		//ptr=ColumnPtr(new Column<bool>(name,BOOLEAN));
		cout << "Fatal Error! invalid AttributeType: " << type << " for Column: " << name 
			  << " Note: bool is currently not supported, will be added again in the future!"<< endl;
	}else{
		cout << "Fatal Error! invalid AttributeType: " << type << " for Column: " << name << endl;
	}

	return ptr;
	}

}; //end namespace CogaDB

