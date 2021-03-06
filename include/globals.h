#ifndef __GLOBALS_H__
# define __GLOBALS_H__

# include <boost/program_options.hpp>

# include "sets.h"
# include "typedefs.h"

extern OperatorStruct	g_operators[];

extern RulesSet	g_rules;
extern FactsSet	g_facts;
extern FactsSet	g_initial_facts;
extern RulesSet	g_used_rules;
extern RulesSet	g_fired_rules;
extern StringVector	g_wishes;

namespace po = boost::program_options;

extern po::variables_map	g_vm;
extern po::options_description	g_desc;

#endif // !__GLOBALS_H__
