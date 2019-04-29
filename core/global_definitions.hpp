
#pragma once

#include <vector>
#include <list>
#include <map>
#include <string>
#include <boost/any.hpp>

#include <boost/shared_ptr.hpp>

namespace shared_pointer_namespace = boost; //std::tr1

namespace CoGaDB{

#define EOL    -2147483648

enum AttributeType{INT,FLOAT,VARCHAR,BOOLEAN};

enum ComputeDevice{CPU,GPU};

enum AggregationMethod{SUM,MIN,MAX};

enum ValueComparator{LESSER,GREATER,EQUAL};

enum SortOrder{ASCENDING,DESCENDING};

enum Operation{SELECTION,PROJECTION,JOIN,GROUPBY,SORT,COPY,AGGREGATION,FULL_SCAN,INDEX_SCAN};

enum JoinAlgorithm{SORT_MERGE_JOIN,NESTED_LOOP_JOIN,HASH_JOIN};

enum MaterializationStatus{MATERIALIZE,LOOKUP};

enum ParallelizationMode{SERIAL,PARALLEL};



enum DebugMode{quiet=1,
					verbose=0,
					debug=0,
					print_time_measurement=0};


//enum DebugMode{quiet=0,
//					verbose=1,
//					debug=1};

typedef unsigned int TID;

typedef std::pair<TID,TID> TID_Pair;

typedef std::pair<AttributeType,std::string> Attribut;

typedef std::list<Attribut> TableSchema;

typedef std::vector<boost::any> Tuple;

//struct Attribut {

//	AttributeType type_;
//	std::string name_;
//	ColumnPtr column_;

//	AttributeType& first;
//	std::string& second;

//}


}; //end namespace CogaDB

