#ifndef __FUNCTIONS_H__
# define __FUNCTIONS_H__

# include <boost/logic/tribool.hpp>

# include "typedefs.h"
# include "node.h"

using namespace boost::logic;

tribool	bool_expression(Node* exp);
tribool	truth_value(Fact F);
tribool	bool_conclusion(tribool condition, Node* exp, Fact F);
tribool	operations(OperatorEnum op, tribool a, tribool b);
void	facts_parsing(bool value);
void	files_parsing();
void	print_out_facts_table();
void	print_out_rules_table(bool tree = false);
void	print_out_fired_rules();
void	prepare_fact(std::string& expression, std::string& conclusion);
void	prepare_rule(std::string& expression, std::string& conclusion);
void	fill_out_line(std::string line);
void	options_parsing(int ac, char** av);
std::string	bool_to_string(tribool b);

#endif // !__FUNCTIONS_H__
