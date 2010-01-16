#ifndef __SET_H__
# define __SET_H__

# include <map>
# include <vector>

# include "node.h"

typedef Node	Rule;

typedef std::vector< Rule* > RulesSet;
typedef std::map< Fact, bool > FactsSet;
// bool	truth_value(Fact F, RulesSet &rules, FactsSet &facts);
typedef std::vector< std::string >	StringVector;

#endif // !__SET_H__
