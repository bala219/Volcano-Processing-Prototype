/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   unittest.hpp
 * Author: ameister
 *
 * Created on 3. Juni 2016, 15:59
 */

#ifndef UNITTEST_HPP
#define UNITTEST_HPP

#include <string>
#include <core/global_definitions.hpp>
#include <core/base_column.hpp>
#include <core/column_base_typed.hpp>
#include <core/column.hpp>
#include <core/compressed_column.hpp>
#include <processing/operator.hpp>


using namespace CoGaDB;

template<typename T>
const T get_rand_value() {
	return 0;
}

#pragma GCC diagnostic ignored "-Wignored-qualifiers"
template<>
const int get_rand_value() {
	return rand() % 100;
}

#pragma GCC diagnostic ignored "-Wignored-qualifiers"
template<>
const float get_rand_value() {
	return float(rand() % 10000) / 100;
}

template<>
const std::string get_rand_value() {
	std::string characterfield="abcdefghijklmnopqrstuvwxyz";

	std::string s;
	for(unsigned int i=0;i<10;i++){
		s.push_back( characterfield[rand() % characterfield.size()]);
	}
	return s;
}

template<class T>
void fill_column(boost::shared_ptr<ColumnBaseTyped<T> > col, std::vector<T>& reference_data) {
	for(unsigned int i = 0;i < reference_data.size(); i++){
		reference_data[i] = get_rand_value<T>();
	}

	for (unsigned int i = 0; i < reference_data.size(); i++) {
		col->insert(reference_data[i]);
	}
	std::cout << "Size in Bytes: " << col->getSizeinBytes() << std::endl;
}

template<class T>
bool equals(std::vector<T> reference_data, boost::shared_ptr<ColumnBaseTyped<T> > col) {
	for (unsigned int i = 0; i < reference_data.size(); i++) {
		T col_value = (*col)[i];
		if (reference_data[i] != col_value) {
			std::cout << "Fatal Error! In Unittest: read invalid data" << std::endl;
			std::cout << "Column: '" << col->getName() 
				<< "' TID: '"<< i 
				<< "' Expected Value: '" << reference_data[i] 
				<< "' Actual Value: '" << col_value << "'" 
				<< std::endl;
			return false;
		}
	}
	return true;
}

template<class T>
bool test_column(boost::shared_ptr<ColumnBaseTyped<T> > col, boost::shared_ptr<ColumnBaseTyped<T> > col_new, std::vector<T>& reference_data) {
	/****** BASIC INSERT TEST ******/
	std::cout << "BASIC INSERT TEST: Filling column with data..."; // << std::endl;	
	//col->insert(reference_data.begin(),reference_data.end()); 
	
	if (reference_data.size() != col->size()) { 
		std::cout << "Fatal Error! In Unittest: invalid data size" << std::endl;
		return false;
	}

	if (!equals(reference_data, col)) {
		std::cerr << "BASIC INSERT TEST FAILED!" << std::endl;
		return false;
	}

	std::cout << std::endl;

	std::cout << "SUCCESS" << std::endl;
	/****** VIRTUAL COPY CONSTRUCTOR TEST ******/
	std::cout << "VIRTUAL COPY CONSTRUCTOR TEST...";

	//boost::shared_ptr<DictionaryCompressedColumn<int> > compressed_col (new DictionaryCompressedColumn<int>("compressed int column",INT));
	//compressed_col->insert(reference_data.begin(),reference_data.end()); 

	ColumnPtr copy = col->copy();
	if(!copy) { 
		std::cerr << std::endl << "VIRTUAL COPY CONSTRUCTOR TEST FAILED!" << std::endl;	
		return false;
	}	
	bool result = *(boost::static_pointer_cast<ColumnBaseTyped<T> >(copy)) == *(boost::static_pointer_cast<ColumnBaseTyped<T> >(col));
	if (!result) { 
		std::cerr << std::endl << "VIRTUAL COPY CONSTRUCTOR TEST FAILED!" << std::endl;	
		return false;
	}	
	std::cout << "SUCCESS"<< std::endl;
	/****** UPDATE TEST ******/
	TID tid = rand() % 100;
	T new_value = get_rand_value<T>();
	std::cout << "UPDATE TEST: Update value on Position '" << tid << "' to new value '" << new_value << "'..."; // << std::endl;

	reference_data[tid] = new_value;

	col->update(tid, new_value);

	if (!equals(reference_data, col)) {
		std::cerr << "UPDATE TEST FAILED!" << std::endl;	
		return false;
	}
	std::cout << "SUCCESS"<< std::endl;
	/****** DELETE TEST ******/
	{
		TID tid = rand() % 100;

		std::cout << "DELETE TEST: Delete value on Position '" << tid << "'..."; // << std::endl;

		reference_data.erase(reference_data.begin()+tid);

		col->remove(tid);

		if (!equals(reference_data, col)) {
			std::cerr << "DELETE TEST FAILED!" << std::endl;
			return false;
		}
		std::cout << "SUCCESS"<< std::endl;
	}
	/****** STORE AND LOAD TEST ******/
	{
		std::cout << "STORE AND LOAD TEST: store column data on disc and load it..."; // << std::endl;
		col->store("data/");

		col->clearContent();
		if(col->size() != 0) {
			std::cout << "Fatal Error! 'col->size()' returned non zero after call to 'col->clearContent()'\nTEST FAILED" << std::endl;
			return false;
		}

		//boost::shared_ptr<Column<int> > col2 (new Column<int>("int column",INT));
		col_new->load("data/");

		if (!equals(reference_data, col_new)) {
			std::cerr << "STORE AND LOAD TEST FAILED!" << std::endl;	
			return false;
		}
		std::cout << "SUCCESS"<< std::endl;
	}

	return true;
}

template<typename ValueType>
AttributeType getAttributeType(){
    return INT;
}

template< >
AttributeType getAttributeType<int>(){
    return INT;
}
template< >
AttributeType getAttributeType<float>(){
    return FLOAT;
}

template< >
AttributeType getAttributeType<std::string>(){
    return VARCHAR;
}

template<typename ValueType>
std::string getAttributeString(){
    return "unknown column";
}

template<>
std::string getAttributeString<int>(){
    return "int column";
}
template<>
std::string  getAttributeString<float>(){
    return "float column";
}

template<>
std::string  getAttributeString<std::string>(){
    return "string column";
}

template<template<typename> class ColumnType, typename ValueType>
bool unittest() {
	std::cout << "RUN Unittest for Column with BaseType ColumnBaseTyped<int> >" << std::endl;
        
         boost::shared_ptr<ColumnType<ValueType> > col_one (new ColumnType<ValueType>(getAttributeString<ValueType>(),getAttributeType<ValueType>()));
         boost::shared_ptr<ColumnType<ValueType> > col_two (new ColumnType<ValueType>(getAttributeString<ValueType>(),getAttributeType<ValueType>()));
	
	std::vector<ValueType> reference_data(100);

	fill_column<ValueType>(col_one, reference_data);
	return test_column<ValueType>(col_one, col_two, reference_data);
}



bool processingTest(){

	std::vector<int> reference_data(100);
	fill_column<int>(column_ptr, reference_data);

	iterate();
	return true;

}

#endif /* UNITTEST_HPP */


