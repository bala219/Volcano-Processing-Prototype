
#pragma once
//STL includes
#include <typeinfo>
//boost includes
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
//CoGaDB includes
#include <core/global_definitions.hpp>

namespace CoGaDB{
/* \brief a PositionList is an STL vector of TID values*/
typedef std::vector<TID> PositionList;
/* \brief a PositionListPtr is a a references counted smart pointer to a PositionList object*/
typedef shared_pointer_namespace::shared_ptr<PositionList> PositionListPtr;
/* \brief a PositionListPair is an STL pair consisting of two PositionListPtr objects
 *  \details This type is returned by binary operators, e.g., joins*/
typedef std::pair<PositionListPtr,PositionListPtr> PositionListPair;
/* \brief a PositionListPairPtr is a a references counted smart pointer to a PositionListPair object*/
typedef shared_pointer_namespace::shared_ptr<PositionListPair> PositionListPairPtr;

class Table; //forward declaration

/*!
 * 
 * 
 *  \brief     This class represents a generic column, is the base class for all column classes and allows a uniform handling of columns. 
 *  \details   This class is indentended to be a base class, so it has a virtual destruktor and pure virtual methods, which need to be implemented in a derived class.
 *  \author    Sebastian Breß
 *  \version   0.2
 *  \date      2013
 *  \copyright GNU LESSER GENERAL PUBLIC LICENSE - Version 3, http://www.gnu.org/licenses/lgpl-3.0.txt
 */

class ColumnBase{
	public:
	/*! \brief defines a smart pointer to a ColumnBase Object*/
	typedef shared_pointer_namespace::shared_ptr<ColumnBase> ColumnPtr;
	/***************** constructors and destructor *****************/
	ColumnBase(const std::string& name, AttributeType db_type);
	virtual ~ColumnBase();
	/***************** methods *****************/	
	/*! \brief appends a value new_Value to end of column
	 *  \return true for sucess and false in case an error occured*/
	virtual bool insert(const boost::any& new_Value)=0;
	/*! \brief updates the value on position tid with a value new_Value
	 *  \return true for sucess and false in case an error occured*/
	virtual bool update(TID tid, const boost::any& new_Value)=0;
	/*! \brief updates the values specified by the position list with a value new_Value
	 *  \return true for sucess and false in case an error occured*/	
	virtual bool update(PositionListPtr tids, const boost::any& new_value) = 0;		
	/*! \brief deletes the value on position tid
	 *  \return true for sucess and false in case an error occured*/
	virtual bool remove(TID tid)=0;
	/*! \brief deletes the values defined in the position list
	 *  \details assumes tid list is sorted ascending
	 *  \return true for sucess and false in case an error occured*/		
	virtual bool remove(PositionListPtr tid)=0;	
	/*! \brief deletes all values stored in the column
	 *  \return true for sucess and false in case an error occured*/	
	virtual bool clearContent()=0;
	/*! \brief generic function for fetching a value form a column (slow)
	 *  \details check whether the object is valid (e.g., when a tid is not valid, then the returned object is invalid as well) 
	 *  \return object of type boost::any containing the value on position tid*/
	virtual const boost::any get(TID tid)=0; //not const, because operator [] does not provide const return type and the child classes rely on []
	/*! \brief prints the content of a column*/
	virtual void print() const throw()=0;
	/*! \brief returns the number of values (rows) in a column*/	
	virtual size_t size() const throw()=0;
	/*! \brief returns the size in bytes the column consumes in main memory*/	
	virtual unsigned int getSizeinBytes() const throw()=0;
	/*! \brief virtual copy constructor
	 * \return a ColumnPtr to an exakt copy of the current column*/	
	virtual const ColumnPtr copy() const=0;
	/***************** relational operations on Columns which return a PositionListPtr/PositionListPairPtr *****************/
	/*! \brief sorts a column w.r.t. a SortOrder
	 * \return PositionListPtr to a PositionList, which represents the result*/		
	virtual const PositionListPtr sort(SortOrder order=ASCENDING)= 0; 
	/*! \brief filters the values of a column according to a filter condition consisting of a comparison value and a ValueComparator (=,<,>)
	 * \return PositionListPtr to a PositionList, which represents the result*/		
	virtual const PositionListPtr selection(const boost::any& value_for_comparison, const ValueComparator comp)= 0;
	/*! \brief filters the values of a column in parallel according to a filter condition consisting of a comparison value and a ValueComparator (=,<,>)
	 * \details the additional parameter specifies the number of threads that may be used to perform the operation
	 * \return PositionListPtr to a PositionList, which represents the result*/		
	virtual const PositionListPtr parallel_selection(const boost::any& value_for_comparison, const ValueComparator comp, unsigned int number_of_threads) = 0;
	/*! \brief joins two columns using the hash join algorithm
	 * \return PositionListPairPtr to a PositionListPair, which represents the result*/		
	virtual const PositionListPairPtr hash_join(ColumnPtr join_column)=0;
	/*! \brief joins two columns using the sort merge join algorithm
	 * \return PositionListPairPtr to a PositionListPair, which represents the result*/		
	virtual const PositionListPairPtr sort_merge_join(ColumnPtr join_column)=0;
	/*! \brief joins two columns using the nested loop join algorithm
	 * \return PositionListPairPtr to a PositionListPair, which represents the result*/			
	virtual const PositionListPairPtr nested_loop_join(ColumnPtr join_column)=0;
	/***************** column algebra operations *****************/	
	/*! \brief adds constant to column
	 *  \details for all indeces i holds the following property: B[i]=A[i]+new_Value*/		
	virtual bool add(const boost::any& new_Value) = 0;
	/*! \brief vector addition of two columns
	 *  \details for all indeces i holds the following property: C[i]=A[i]+B[i]*/				
	virtual bool add(ColumnPtr column) = 0;
	/*! \brief substracts constant from column
	 *  \details for all indeces i holds the following property: B[i]=A[i]-new_Value*/		
	virtual bool minus(const boost::any& new_Value) = 0;
	/*! \brief vector substraction of two columns
	 *  \details for all indeces i holds the following property: C[i]=A[i]-B[i]*/		
	virtual bool minus(ColumnPtr column) = 0;	
	/*! \brief multiply constant with column
	 *  \details for all indeces i holds the following property: B[i]=A[i]*new_Value*/	
	virtual bool multiply(const boost::any& new_Value) = 0;
	/*! \brief multiply two columns A and B
	 *  \details for all indeces i holds the following property: C[i]=A[i]*B[i]*/			
	virtual bool multiply(ColumnPtr column) = 0;
	/*! \brief devide values in column by a constant
	 *  \details for all indeces i holds the following property: B[i]=A[i]/new_Value*/				
	virtual bool division(const boost::any& new_Value) = 0;	
	/*! \brief devide column A with column B
	 *  \details for all indeces i holds the following property: C[i]=A[i]/B[i]*/				
	virtual bool division(ColumnPtr column) = 0;	
	/***************** persistency operations *****************/		
	/*! \brief store a column on the disc
	 *  \return true for sucess and false in case an error occured*/	
	virtual bool store(const std::string& path) = 0;
	/*! \brief load column from disc
	 *  \details calling load on a column that is not empty yields undefined behaviour
	 *  \return true for sucess and false in case an error occured*/		
	virtual bool load(const std::string& path) = 0;
	/*! \brief use this method to determine whether the column is materialized or a Lookup Column
	 * \return true in case the column is storing the plain values (without compression) and false in case the column is a LookupColumn.*/		
	/***************** misc operations *****************/	
	virtual bool isMaterialized() const  throw() = 0;
	/*! \brief use this method to determine whether the column is materialized or a Lookup Column
	 * \return true in case the column is storing the compressed values and false otherwise.*/			
	virtual bool isCompressed() const  throw() = 0;	
	/*! \brief returns type information of internal values*/
	virtual const std::type_info& type() const throw() = 0;
	/*! \brief returns database type of column (as defined in "SQL" statement)*/
	AttributeType getType() const throw();
	/*! \brief returns attribute name of column  
		\return attribute name of column*/
	const std::string getName() const throw();

	protected:
	/*! \brief attribute name of the column*/	
	std::string name_;
	/*! \brief database type of the column*/		
	AttributeType db_type_;
//	Table& table_;
};

/*! \brief makes a smart pointer to a ColumnBase Object visible in the namespace*/
typedef ColumnBase::ColumnPtr ColumnPtr;

typedef std::vector<ColumnPtr> ColumnVector;
typedef shared_pointer_namespace::shared_ptr<ColumnVector> ColumnVectorPtr;

/*! \brief Column factory function, creates an empty materialized column*/
const ColumnPtr createColumn(AttributeType type, const std::string& name);

}; //end namespace CogaDB


//extend boost namespace to add serialization feature to my own types
namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, std::pair<CoGaDB::AttributeType,std::string> & pair, const unsigned int ) //version)
{
	
    ar & pair.first;
    ar & pair.second;
    
}

} // namespace serialization
} // namespace boost

