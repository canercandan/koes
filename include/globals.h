#ifndef __GLOBALS_H__
# define __GLOBALS_H__

# include <boost/program_options.hpp>

# include "sets.h"
# include "typedefs.h"

extern OperatorStruct	g_operators[];
extern BooleanStruct	g_booleans[];

extern RulesSet	g_rules;
extern FactsSet	g_facts;

namespace po = boost::program_options;

extern po::variables_map	g_vm;
extern po::options_description	g_desc;

extern RulesSet	used_rules;
#endif // !__GLOBALS_H__
