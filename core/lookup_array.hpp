
#pragma once

#include <core/column_base_typed.hpp>
#include <core/column.hpp>
#include <lookup_table/lookup_column.hpp>
#include <core/base_table.hpp>

namespace CoGaDB{

/*!
 * 
 * 
 *  \brief     A LookupArray is a LookupColumn which is applied on a materialized column (of the table that is indexed by the Lookup column) and hence has a Type. 
 * 				This class represents a column with type T, which is essentially a tid list describing which values of a typed materialized column are included in the LookupArray.
 *  \details   This class is indentended to be a base class, so it has a virtual destruktor and pure virtual methods, which need to be implemented in a derived class. 
 *  \author    Sebastian Breß
 *  \version   0.2
 *  \date      2013
 *  \copyright GNU LESSER GENERAL PUBLIC LICENSE - Version 3, http://www.gnu.org/licenses/lgpl-3.0.txt
 */	

template<class T>
class LookupArray : public ColumnBaseTyped<T>{
	public:
	/***************** constructors and destructor *****************/
	LookupArray(const std::string& name, AttributeType db_type, ColumnPtr column, PositionListPtr tids);
	virtual ~LookupArray();

	virtual bool insert(const boost::any& new_Value);
	virtual bool insert(const T& new_Value);
	virtual bool update(TID tid, const boost::any& new_value);
	virtual bool update(PositionListPtr tid, const boost::any& new_value);	
	
	virtual bool remove(TID tid);
	//assumes tid list is sorted ascending
	virtual bool remove(PositionListPtr tid);
	virtual bool clearContent();

	virtual const boost::any get(TID tid);
	//virtual const boost::any* const getRawData()=0;
	virtual void print() const throw();
	virtual size_t size() const throw();
	virtual unsigned int getSizeinBytes() const throw();

	virtual const ColumnPtr copy() const;

	virtual bool store(const std::string& path);
	virtual bool load(const std::string& path);
	virtual bool isMaterialized() const  throw();
	virtual bool isCompressed() const  throw();		
	/*! \brief returns type information of internal values*/
	virtual T& operator[](const int index);
	private:
	shared_pointer_namespace::shared_ptr<ColumnBaseTyped<T> > column_;
	PositionListPtr tids_;
};

	//typedef shared_pointer_namespace::shared_ptr<LookupArray> LookupArrayPtr;

/***************** Start of Implementation Section ******************/

	
	template<class T>
	LookupArray<T>::LookupArray(const std::string& name, AttributeType db_type, ColumnPtr column, PositionListPtr tids) 
						: ColumnBaseTyped<T>(name, db_type),
						  column_( shared_pointer_namespace::static_pointer_cast<ColumnBaseTyped<T> > (column) ),
						  tids_(tids) {
	
		assert(column_!=NULL);
		assert(tids_!=NULL);
		assert(db_type==column->getType());

	}

	template<class T>
	LookupArray<T>::~LookupArray(){

	}

	template<class T>
	bool LookupArray<T>::insert(const boost::any& ){
		return false;
	}
	template<class T>
	bool LookupArray<T>::insert(const T&){
		return false;
	}
	template<class T>
	bool LookupArray<T>::update(TID , const boost::any& ){
		return  false;
	}

	template<class T>
	bool LookupArray<T>::update(PositionListPtr, const boost::any&){
		return  false;			
	}
	
	template<class T>
	bool LookupArray<T>::remove(TID ){

		return false;
	}
	
	//assumes tid list is sorted ascending
	template<class T>
	bool LookupArray<T>::remove(PositionListPtr){
		return false;
	}

	template<class T>
	bool LookupArray<T>::clearContent(){
		return false;
	}
	
	template<class T>
	const boost::any LookupArray<T>::get(TID tid){
		return boost::any((*this)[tid]);
	}

	template<class T>
	void LookupArray<T>::print() const throw(){
		
		const shared_pointer_namespace::shared_ptr<ColumnBaseTyped<T> > column = column_;
		const PositionListPtr tids = tids_;

		std::cout << "Lookup Array for Column " << column_->getName() << " ";
		if(column_->isMaterialized()){
			std::cout << "which is a materialized Column" << std::endl;
		}else{
			std::cout << "which is a LookupArray of a Lookup column" << std::endl;
		}
		std::cout << "| values | Translatetion TIDS | Index in Lookup Table |" << std::endl;
		for(unsigned int i=0;i<tids->size();i++){
			std::cout << "| " << (*column_)[(*tids_)[i]] << " | " << (*tids_)[i] << " | " << i << " |" << std::endl;
		}


	}
	template<class T>
	size_t LookupArray<T>::size() const throw(){

		return tids_->size();
	}
	template<class T>
	const ColumnPtr LookupArray<T>::copy() const{
		PositionListPtr new_tids (new PositionList(*tids_));
		return ColumnPtr(new LookupArray<T>(this->name_,this->db_type_,this->column_,new_tids));
	}
	/***************** relational operations on LookupArrays which return lookup tables *****************/
//	template<class T>
//	const std::vector<TID> LookupArray<T>::sort(const ComputeDevice comp_dev) const {

//		return std::vector<TID>();
//	}

//	template<class T> 
//	const std::vector<TID> LookupArray<T>::selection(const boost::any& value_for_comparison, const ValueComparator comp, const ComputeDevice comp_dev) const {
//		return std::vector<TID>();
//	}
//	//join algorithms
//	template<class T>
//	const std::vector<TID_Pair> LookupArray<T>::sort_merge_join(ColumnPtr join_LookupArray, const ComputeDevice comp_dev) const{
//		return std::vector<TID_Pair>();
//	}
//	template<class T>
//	const std::vector<TID_Pair> LookupArray<T>::nested_loop_join(ColumnPtr join_LookupArray, const ComputeDevice comp_dev) const{
//		return false;
//	}
	template<class T>
	bool LookupArray<T>::store(const std::string&){
		return false;
	}
	template<class T>
	bool LookupArray<T>::load(const std::string&){
		return false;
	}
	template<class T>
	bool LookupArray<T>::isMaterialized() const  throw(){
		return false;
	}
	template<class T>
	bool LookupArray<T>::isCompressed() const  throw(){
		return false;
	}
	template<class T>
	T& LookupArray<T>::operator[](const int index){
		return (*column_)[(*tids_)[index]];
	}

	template<class T>
	unsigned int LookupArray<T>::getSizeinBytes() const throw(){
		return tids_->capacity()*sizeof(typename PositionList::value_type);
	}

/***************** End of Implementation Section ******************/



}; //end namespace CogaDB

