#ifndef __SET_H__
# define __SET_H__

# include <map>
# include <vector>

# include "node.h"

typedef Node	Rule;

typedef std::vector< Rule* > RulesSet;
typedef std::map< Fact, bool > FactsSet;

#endif // !__SET_H__
