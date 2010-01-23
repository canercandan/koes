#ifndef __FUNCTIONS_H__
# define __FUNCTIONS_H__

# include "typedefs.h"
# include "node.h"

Boolean	bool_expression(Node* exp);
Boolean	truth_value(Fact F);
Boolean	bool_conclusion(Boolean condition, Node* exp, Fact F);
Boolean	operations(OperatorEnum op, Boolean a, Boolean b);
void	facts_parsing(bool value);
void	files_parsing();
void	print_out_facts_table();
void	print_out_rules_table(bool tree = false);
void	prepare_fact(std::string& expression, std::string& conclusion);
void	prepare_rule(std::string& expression, std::string& conclusion);
void	fill_out_line(std::string line);
void	options_parsing(int ac, char** av);

#endif // !__FUNCTIONS_H__
