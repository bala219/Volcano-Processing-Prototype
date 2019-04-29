
#pragma once

#include <core/base_column.hpp>
#include <iostream>

#include <utility>
#include <functional>
#include <algorithm>

#include <boost/unordered_map.hpp>
#include <boost/any.hpp>

//#include <core/column.hpp>

/*! \brief The global namespace of the programming tasks, to avoid name claches with other libraries.*/
namespace CoGaDB{

/*!
 * 
 * 
 *  \brief     This class represents a column with type T, is the base class for all typed column classes and allows a uniform handling of columns of a certain type T. 
 *  \details   This class is indentended to be a base class, so it has a virtual destruktor and pure virtual methods, which need to be implemented in a derived class. 
 * 				Furthermore, it declares pure virtual methods to allow a generic handling of typed columns, e.g., operator[]. All algorithms can be applied to a typed 
 * 				column, because of this operator. This abstracts from a columns implementation detail, e.g., whether they are compressed or not. 
 *  \author    Sebastian Breß
 *  \version   0.2
 *  \date      2013
 *  \copyright GNU LESSER GENERAL PUBLIC LICENSE - Version 3, http://www.gnu.org/licenses/lgpl-3.0.txt
 */	
	
template<class T>
class ColumnBaseTyped : public ColumnBase{
	public:
	//typedef boost::shared_ptr<ColumnBaseTyped> ColumnPtr;
	/***************** constructors and destructor *****************/
	ColumnBaseTyped(const std::string& name, AttributeType db_type);
	virtual ~ColumnBaseTyped();

	virtual bool insert(const boost::any& new_Value)=0;
	virtual bool insert(const T& new_Value)=0;
	virtual bool update(TID tid, const boost::any& new_value) = 0;
	virtual bool update(PositionListPtr tid, const boost::any& new_value) = 0;	
	
	virtual bool remove(TID tid)=0;
	//assumes tid list is sorted ascending
	virtual bool remove(PositionListPtr tid)=0;
	virtual bool clearContent()=0;

	virtual const boost::any get(TID tid)=0;
	//virtual const boost::any* const getRawData()=0;
	virtual void print() const throw()=0;
	virtual size_t size() const throw()=0;
	virtual unsigned int getSizeinBytes() const throw()=0;

	virtual const ColumnPtr copy() const=0;
	/***************** relational operations on Columns which return lookup tables *****************/
	virtual const PositionListPtr sort(SortOrder order); 
	virtual const PositionListPtr selection(const boost::any& value_for_comparison, const ValueComparator comp);
	virtual const PositionListPtr parallel_selection(const boost::any& value_for_comparison, const ValueComparator comp, unsigned int number_of_threads);
	//join algorithms
	virtual const PositionListPairPtr hash_join(ColumnPtr join_column);
	virtual const PositionListPairPtr sort_merge_join(ColumnPtr join_column);
	virtual const PositionListPairPtr nested_loop_join(ColumnPtr join_column);


	virtual bool add(const boost::any& new_Value);
	//vector addition between columns			
	virtual bool add(ColumnPtr join_column);

	virtual bool minus(const boost::any& new_Value);
	virtual bool minus(ColumnPtr join_column);	

	virtual bool multiply(const boost::any& new_Value);
	virtual bool multiply(ColumnPtr join_column);

	virtual bool division(const boost::any& new_Value);	
	virtual bool division(ColumnPtr join_column);	

	//template <typename U, typename BinaryOperator>
	//std::pair<ColumnPtr,ColumnPtr> aggregate_by_keys(ColumnBaseTyped<U>* keys, BinaryOperator binary_op) const;

	virtual bool store(const std::string& path) = 0;
	virtual bool load(const std::string& path) = 0;
	virtual bool isMaterialized() const  throw() = 0;
	virtual bool isCompressed() const  throw() = 0;	
	/*! \brief returns type information of internal values*/
	virtual const std::type_info& type() const throw();
	/*! \brief defines operator[] for this class, which enables the user to thread all typed columns as arrays.
	 * \details Note that this method is pure virtual, so it has to be defined in a derived class. 
	 * \return a reference to the value at position index
	 * */
	virtual T& operator[](const int index) = 0;
	inline bool operator==(ColumnBaseTyped<T>& column);
};


	template<class T>
	ColumnBaseTyped<T>::ColumnBaseTyped(const std::string& name, AttributeType db_type) : ColumnBase(name,db_type){

	}

	template<class T>
	ColumnBaseTyped<T>::~ColumnBaseTyped(){

	}

template<class T>
const std::type_info& ColumnBaseTyped<T>::type() const throw(){
	return typeid(T);
}

template<class T>
const PositionListPtr ColumnBaseTyped<T>::sort(SortOrder order){

	PositionListPtr ids = PositionListPtr( new PositionList());
		std::vector<std::pair<T,TID> > v;

		for(unsigned int i=0;i<this->size();i++){
			v.push_back (std::pair<T,TID>((*this)[i],i) );
		}

		//TODO: change implementation, so that no copy operations are required -> use boost zip iterators!

	  	if(order==ASCENDING){
			//tbb::parallel_sort(v.begin(),v.end(),std::less_equal<std::pair<T,TID> >());
	  		std::stable_sort(v.begin(),v.end(),std::less_equal<std::pair<T,TID> >());
		}else if(order==DESCENDING){
			//tbb::parallel_sort(v.begin(),v.end(),std::greater_equal<std::pair<T,TID> >());
			std::stable_sort(v.begin(),v.end(),std::greater_equal<std::pair<T,TID> >()); 
		}else{
			std::cout << "FATAL ERROR: ColumnBaseTyped<T>::sort(): Unknown Sorting Order!" << std::endl;
		}

		for(unsigned int i=0;i<v.size();i++){
			ids->push_back(v[i].second);
		}

		return ids;
}


template<class T>
const PositionListPtr ColumnBaseTyped<T>::parallel_selection(const boost::any&, const ValueComparator, unsigned int){

		PositionListPtr result_tids( new PositionList());

		return result_tids;
}



template<class T>
const PositionListPtr ColumnBaseTyped<T>::selection(const boost::any& value_for_comparison, const ValueComparator comp){
		if(value_for_comparison.type()!=typeid(T)){
			std::cout << "Fatal Error!!! Typemismatch for column " << name_ << std::endl;
			std::cout << "File: " << __FILE__ << " Line: " << __LINE__ << std::endl;
			exit(-1);
		}
			
		T value = boost::any_cast<T>(value_for_comparison);
	

		PositionListPtr result_tids;

				result_tids = PositionListPtr(new PositionList());

				if(!quiet) std::cout << "Using CPU for Selection..." << std::endl;
				for(TID i=0;i<this->size();i++){
		
					//boost::any value = column->get(i);
					//val = values_[i];
			
					if(comp==EQUAL){
					if(value==(*this)[i]){
						//result_table->insert(this->fetchTuple(i));
						result_tids->push_back(i);
					}
					}else if(comp==LESSER){
					if((*this)[i]<value){
						//result_table->insert(this->fetchTuple(i));
						result_tids->push_back(i);
					}
					}else if(comp==GREATER){
					if((*this)[i]>value){
						result_tids->push_back(i);
						//result_table->insert(this->fetchTuple(i));
					}
					}else{

					}
				}	

	    //}
	    return result_tids;
}

	
	template<class T>
	const PositionListPairPtr ColumnBaseTyped<T>::hash_join(ColumnPtr join_column_){

		typedef boost::unordered_multimap<T,TID,boost::hash<T>, std::equal_to<T> > HashTable;

				if(join_column_->type()!=typeid(T)){
					std::cout << "Fatal Error!!! Typemismatch for columns " << this->name_  << " and " << join_column_->getName() << std::endl;
					std::cout << "File: " << __FILE__ << " Line: " << __LINE__ << std::endl;
					exit(-1);
				}
				
				shared_pointer_namespace::shared_ptr<ColumnBaseTyped<T> > join_column = shared_pointer_namespace::static_pointer_cast<ColumnBaseTyped<T> >(join_column_); //static_cast<IntColumnPtr>(column1);

				PositionListPairPtr join_tids( new PositionListPair());
				join_tids->first = PositionListPtr( new PositionList() );
				join_tids->second = PositionListPtr( new PositionList() );


	//create hash table
	HashTable hashtable;
	for(unsigned int i=0;i<this->size();i++)	
		hashtable.insert(
								std::pair<T,TID> ((*this)[i],i)
					);

	//probe larger relation
	for(unsigned int i=0;i<join_column->size();i++){
		std::pair<typename HashTable::iterator, typename HashTable::iterator> range =  hashtable.equal_range((*join_column)[i]);
		for(typename HashTable::iterator it=range.first ; it!=range.second;it++){
			if(it->first==(*join_column)[i]){
				join_tids->first->push_back(it->second);
				join_tids->second->push_back(i);
				//cout << "match! " << it->second << ", " << i << "	"  << it->first << endl;
			}
		}
	}

		return join_tids;
	}

	template<class Type>
	const PositionListPairPtr ColumnBaseTyped<Type>::sort_merge_join(ColumnPtr join_column_){

				if(join_column_->type()!=typeid(Type)){
					std::cout << "Fatal Error!!! Typemismatch for columns " << this->name_  << " and " << join_column_->getName() << std::endl;
					std::cout << "File: " << __FILE__ << " Line: " << __LINE__ << std::endl;
					exit(-1);
				}
				
				shared_pointer_namespace::shared_ptr<ColumnBaseTyped<Type> > join_column = shared_pointer_namespace::static_pointer_cast<ColumnBaseTyped<Type> >(join_column_); //static_cast<IntColumnPtr>(column1);

				PositionListPairPtr join_tids( new PositionListPair());
				join_tids->first = PositionListPtr( new PositionList() );
				join_tids->second = PositionListPtr( new PositionList() );

				return join_tids;
	}


	template<class Type>
	const PositionListPairPtr ColumnBaseTyped<Type>::nested_loop_join(ColumnPtr join_column_){
				assert(join_column_!=NULL);
				if(join_column_->type()!=typeid(Type)){
					std::cout << "Fatal Error!!! Typemismatch for columns " << this->name_  << " and " << join_column_->getName() << std::endl;
					std::cout << "File: " << __FILE__ << " Line: " << __LINE__ << std::endl;
					exit(-1);
				}
				
				shared_pointer_namespace::shared_ptr<ColumnBaseTyped<Type> > join_column = shared_pointer_namespace::static_pointer_cast<ColumnBaseTyped<Type> >(join_column_); //static_cast<IntColumnPtr>(column1);

				PositionListPairPtr join_tids( new PositionListPair());
				join_tids->first = PositionListPtr( new PositionList() );
				join_tids->second = PositionListPtr( new PositionList() );

		for(unsigned int i=0;i<this->size();i++){
			for(unsigned int j=0;j<join_column->size();j++){
				if((*this)[i]==(*join_column)[j]){
					if(debug) std::cout << "MATCH: (" << i << "," << j << ")" << std::endl;
					join_tids->first->push_back(i);
					join_tids->second->push_back(j);
				}
			}
		}

		return join_tids;
	}

	template<class T>
	bool ColumnBaseTyped<T>::operator==(ColumnBaseTyped<T>& column){
	  if(this->size()!=column.size()) return false;
	  for(unsigned int i=0;i<this->size();i++){
			if((*this)[i]!=column[i]){ 	
				return false;
			}
	  }
	  return true;
   }

	template<class Type>
	bool ColumnBaseTyped<Type>::add(const boost::any& new_value){
		if(new_value.empty()) return false;
		if(typeid(Type)==new_value.type()){
			 Type value = boost::any_cast<Type>(new_value);
			 //std::transform(myvec.begin(), myvec.end(), myvec.begin(),
          //bind2nd(std::plus<double>(), 1.0));
			 for(unsigned int i=0;i<this->size();i++){
					this->operator[](i)+=value;
			 }
			 return true;
		}
		return false;
	}
	

			
	template<class Type>
	bool ColumnBaseTyped<Type>::add(ColumnPtr column){
		//std::transform ( first, first+5, second, results, std::plus<int>() );		
		shared_pointer_namespace::shared_ptr<ColumnBaseTyped<Type> > typed_column = shared_pointer_namespace::static_pointer_cast<ColumnBaseTyped<Type> >(column);
		if(!column) return false;
		for(unsigned int i=0;i<this->size();i++){
			this->operator[](i)+=typed_column->operator[](i);
		}			
		return true;
	}



	template<class Type>
	bool ColumnBaseTyped<Type>::minus(const boost::any& new_value){
		//shared_pointer_namespace::shared_ptr<ColumnBaseTyped<Type> > typed_column = shared_pointer_namespace::static_pointer_cast<ColumnBaseTyped<Type> >(column);	
		if(new_value.empty()) return false;
		if(typeid(Type)==new_value.type()){
			 Type value = boost::any_cast<Type>(new_value);
			 for(unsigned int i=0;i<this->size();i++){
					this->operator[](i)-=value;
			 }
			 return true;
		}
		return false;
	}
	
	template<class Type>
	bool ColumnBaseTyped<Type>::minus(ColumnPtr column){
		//std::transform ( first, first+5, second, results, std::plus<int>() );		
		shared_pointer_namespace::shared_ptr<ColumnBaseTyped<Type> > typed_column = shared_pointer_namespace::static_pointer_cast<ColumnBaseTyped<Type> >(column);
		if(!column) return false;
		for(unsigned int i=0;i<this->size();i++){
			this->operator[](i)-=typed_column->operator[](i);
		}			
		return true;
	}	


	template<class Type>
	bool ColumnBaseTyped<Type>::multiply(const boost::any& new_value){
		if(new_value.empty()) return false;
		if(typeid(Type)==new_value.type()){
			 Type value = boost::any_cast<Type>(new_value);
			 for(unsigned int i=0;i<this->size();i++){
					this->operator[](i)*=value;
			 }
			 return true;
		}
		return false;
	}
	
	template<class Type>
	bool ColumnBaseTyped<Type>::multiply(ColumnPtr column){
		//std::transform ( first, first+5, second, results, std::plus<int>() );		
		shared_pointer_namespace::shared_ptr<ColumnBaseTyped<Type> > typed_column = shared_pointer_namespace::static_pointer_cast<ColumnBaseTyped<Type> >(column);
		if(!column) return false;
		for(unsigned int i=0;i<this->size();i++){
			this->operator[](i)*=typed_column->operator[](i);
		}			
		return true;
	}



	template<class Type>
	bool ColumnBaseTyped<Type>::division(const boost::any& new_value){
		if(new_value.empty()) return false;
		if(typeid(Type)==new_value.type()){
			 Type value = boost::any_cast<Type>(new_value);
			 //check that we do not devide by zero
			 if(value==0) return false;
			 for(unsigned int i=0;i<this->size();i++){
					this->operator[](i)/=value;
			 }
			 return true;
		}
		return false;
	}
	
	template<class Type>
	bool ColumnBaseTyped<Type>::division(ColumnPtr column){
		//std::transform ( first, first+5, second, results, std::plus<int>() );		
		shared_pointer_namespace::shared_ptr<ColumnBaseTyped<Type> > typed_column = shared_pointer_namespace::static_pointer_cast<ColumnBaseTyped<Type> >(column);
		if(!column) return false;
		for(unsigned int i=0;i<this->size();i++){
			this->operator[](i)/=typed_column->operator[](i);
		}			
		return true;
	}

	//total tempalte specializations, because numeric computations are undefined on strings 
	template<>
	inline bool ColumnBaseTyped<std::string>::add(const boost::any&){ return false;	}
	template<>
	inline bool ColumnBaseTyped<std::string>::add(ColumnPtr){ return false;	}

	template<>
	inline bool ColumnBaseTyped<std::string>::minus(const boost::any&){ return false;	}
	template<>
	inline bool ColumnBaseTyped<std::string>::minus(ColumnPtr){ return false;	}


	template<>
	inline bool ColumnBaseTyped<std::string>::multiply(const boost::any&){ return false;	}
	template<>
	inline bool ColumnBaseTyped<std::string>::multiply(ColumnPtr){ return false;	}
	
	template<>
	inline bool ColumnBaseTyped<std::string>::division(const boost::any&){ return false;	}
	template<>
	inline bool ColumnBaseTyped<std::string>::division(ColumnPtr){ return false;	}

}; //end namespace CogaDB

