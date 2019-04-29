#pragma once

#include <core/column_base_typed.hpp>
#include <iostream>
#include <fstream>

namespace CoGaDB{

template<typename T>
class Column : public ColumnBaseTyped<T>{
	public:
	/***************** constructors and destructor *****************/
	Column(const std::string& name, AttributeType db_type);
	//Column(const Column& column);
	virtual ~Column();

	virtual bool insert(const boost::any& new_value);
	bool insert(const T& new_value);	
	template <typename InputIterator>
	bool insert(InputIterator first, InputIterator last);

	virtual bool update(TID tid, const boost::any& new_value);
	virtual bool update(PositionListPtr tid, const boost::any& new_value);	
	
	virtual bool remove(TID tid);
	//assumes tid list is sorted ascending
	virtual bool remove(PositionListPtr tid);
	virtual bool clearContent();

	virtual const boost::any get(TID tid);
	//virtual const boost::any* const getRawData();
	virtual void print() const throw();
	virtual size_t size() const throw();
	virtual unsigned int getSizeinBytes() const throw();

	virtual const ColumnPtr copy() const;

	virtual bool store(const std::string& path);
	virtual bool load(const std::string& path);
	virtual bool isMaterialized() const  throw();
	virtual bool isCompressed() const  throw();	
	
	virtual T& operator[](const int index);

	std::vector<T>& getContent();

	private:

		struct Type_TID_Comparator {
  			inline bool operator() (std::pair<T,TID> i, std::pair<T,TID> j) { return (i.first<j.first);}
		} type_tid_comparator;

	
	/*! values*/
	std::vector<T> values_;
};



/***************** Start of Implementation Section ******************/

	
	template<class T>
	Column<T>::Column(const std::string& name, AttributeType db_type) : ColumnBaseTyped<T>(name,db_type), type_tid_comparator(), values_(){

	}

//	template<class T>
//	Column<T>::Column(const Column& column) : this->db_type_(column->db_type_), this->values_(column->values_){
//		
//	}

	template<class T>
	Column<T>::~Column(){

	}

	template<class T>
	std::vector<T>& Column<T>::getContent(){
		return values_;
	}


	template<class T>
	bool Column<T>::insert(const boost::any& new_value){
		if(new_value.empty()) return false;
		if(typeid(T)==new_value.type()){
			 T value = boost::any_cast<T>(new_value);
			 values_.push_back(value);
			 return true;
		}
		return false;
	}

	template<class T>
	bool Column<T>::insert(const T& new_value){
		values_.push_back(new_value);
		return true;
	}


	template <typename T> 
	template <typename InputIterator>
	bool Column<T>::insert(InputIterator first, InputIterator last){
		this->values_.insert(this->values_.end(),first,last);
		return true;
	}

	template<class T>
	bool Column<T>::update(TID tid, const boost::any& new_value){
		if(new_value.empty()) return false;
		if(typeid(T)==new_value.type()){
			 T value = boost::any_cast<T>(new_value);
			 values_[tid]=value;
			 return true;
		}else{
			std::cout << "Fatal Error!!! Typemismatch for column " << this->name_ << std::endl; 
		}
		return false;
	}

	template<class T>
	bool Column<T>::update(PositionListPtr tids, const boost::any& new_value){
		if(!tids)
			return false;
	if(new_value.empty()) return false;
		if(typeid(T)==new_value.type()){
			 T value = boost::any_cast<T>(new_value);
			 for(unsigned int i=0;i<tids->size();i++){
				TID tid=(*tids)[i];
				values_[tid]=value;
			 }
			 return true;
		}else{
			std::cout << "Fatal Error!!! Typemismatch for column " << this->name_ << std::endl; 
		}
		return false;		
	}
	



	template<class T>
	bool Column<T>::remove(TID tid){
		values_.erase(values_.begin()+tid);
		return true;
	}
	
	template<class T>
	bool Column<T>::remove(PositionListPtr tids){
		if(!tids)
			return false;
		//test whether tid list has at least one element, if not, return with error
		if(tids->empty())
			return false;		

		//assert();

		typename PositionList::reverse_iterator rit;

		for (rit = tids->rbegin(); rit!=tids->rend(); ++rit)
			values_.erase(values_.begin()+(*rit));

		/*
		//delete tuples in reverse order, otherwise the first deletion would invalidate all other tids
		unsigned int i=tids->size()-1;
		while(true)	
			TID = (*tids)[i];
			values_.erase(values_.begin()+tid);		
			if(i==0) break;
		}*/
		
		
		return true;			
	}

	template<class T>
	bool Column<T>::clearContent(){
		values_.clear();
		return true;
	}

	template<class T>
	const boost::any Column<T>::get(TID tid){
		if(tid<values_.size())
 			return boost::any(values_[tid]);
		else{
			std::cout << "fatal Error!!! Invalid TID!!! Attribute: " << this->name_ << " TID: " << tid  << std::endl;
		}
		return boost::any();
	}

	template<class T>
	void Column<T>::print() const throw(){
		std::cout << "| " << this->name_ << " |" << std::endl;
		std::cout << "________________________" << std::endl;
		for(unsigned int i=0;i<values_.size();i++){
			std::cout << "| " << values_[i] << " |" << std::endl;
		}
	}
	template<class T>
	size_t Column<T>::size() const throw(){
		return values_.size();
	}
	template<class T>
	const ColumnPtr Column<T>::copy() const{
		return ColumnPtr(new Column<T>(*this));
	}
	/***************** relational operations on Columns which return lookup tables *****************/
//	template<class T>
//	const std::vector<TID> Column<T>::sort(const ComputeDevice comp_dev) const {

//		return std::vector<TID>();
//	}

//	template<class T> 
//	const std::vector<TID> Column<T>::selection(const boost::any& value_for_comparison, const ValueComparator comp, const ComputeDevice comp_dev) const {

//		return std::vector<TID>();
//	}
//	//join algorithms
//	template<class T>
//	const std::vector<TID_Pair> Column<T>::sort_merge_join(ColumnPtr join_Column, const ComputeDevice comp_dev) const{

//		return std::vector<TID_Pair>();
//	}
//	template<class T>
//	const std::vector<TID_Pair> Column<T>::nested_loop_join(ColumnPtr join_Column, const ComputeDevice comp_dev) const{

//		return std::vector<TID_Pair>();
//	}
	template<class T>
	bool Column<T>::store(const std::string& path_){
		//string path("data/");
		std::string path(path_);
		path += "/";
		path += this->name_;
		//std::cout << "Writing Column " << this->getName() << " to File " << path << std::endl;
		std::ofstream outfile (path.c_str(),std::ios_base::binary | std::ios_base::out);
		boost::archive::binary_oarchive oa(outfile);

		oa << values_;

		outfile.flush();
		outfile.close();
		return true;
	}
	template<class T>
	bool Column<T>::load(const std::string& path_){
		std::string path(path_);
		//std::cout << "Loading column '" << this->name_ << "' from path '" << path << "'..." << std::endl;
		//string path("data/");
		path += "/";
		path += this->name_;
		
		//std::cout << "Opening File '" << path << "'..." << std::endl;
		std::ifstream infile (path.c_str(),std::ios_base::binary | std::ios_base::in);
		boost::archive::binary_iarchive ia(infile);
		ia >> values_;
		infile.close();


		return true;
	}
	template<class T>
	bool Column<T>::isMaterialized() const  throw(){
		return true;
	}
	
	template<class T>
	bool Column<T>::isCompressed() const  throw(){
		return false;
	}

	template<class T>
	T& Column<T>::operator[](const int index){
		
		return values_[index];
	}

	template<class T>
	unsigned int Column<T>::getSizeinBytes() const throw(){
			return values_.capacity()*sizeof(T);
	}

	//total template specialization
	template<>
	inline unsigned int Column<std::string>::getSizeinBytes() const throw(){
		unsigned int size_in_bytes=0;
		for(unsigned int i=0;i<values_.size();++i){
			size_in_bytes+=values_[i].capacity();
		}
		//return values_.size()*sizeof(T);
		return size_in_bytes;
	}

/***************** End of Implementation Section ******************/




}; //end namespace CogaDB

