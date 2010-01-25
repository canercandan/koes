#ifndef __SET_H__
# define __SET_H__

# include <map>
# include <vector>
# include <boost/logic/tribool.hpp>

# include "node.h"

using namespace boost::logic;

typedef Node	Rule;

typedef std::vector< Rule* > RulesSet;
typedef std::map< Fact, tribool > FactsSet;
typedef std::vector< std::string >	StringVector;

#endif // !__SET_H__
