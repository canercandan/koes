#include "functions.h"
#include "globals.h"
#include "defines.h"

static std::string::size_type	get_operator_position(std::string& expr, std::string::size_type index = 0)
{
  return expr.find_first_of(EXPR_OPERATORS, index);
}

static OperatorEnum	get_operator(Operator& op)
{
  for (int i = 0; g_operators[i].op != -1; ++i)
    if (g_operators[i].desc == op)
      return g_operators[i].op;
  return (OperatorEnum)-1;
}

static Node*	create_binary_tree_from_expression(std::string& expr)
{
  std::string			str(expr);
  std::string::size_type	found = get_operator_position(str);

  if (found == std::string::npos) // in the case where we have only 1 fact into expression
    return new Node(FACT, NULL, NULL, str);

  Node*	node = NULL;
  Node*	root = NULL;

  while (found != std::string::npos)
    {
      Operator	op = str.substr(found, 1);
      Fact	left = str.substr(0, found);
      Node*	newnode = new Node(get_operator(op),
				   new Node(FACT, NULL, NULL, left),
				   NULL);

      if (node != NULL)
	node->right = newnode;
      if (root == NULL)
	root = newnode;
      node = newnode;
      str = str.substr(found + 1);
      found = get_operator_position(str);
    }

  if (node != NULL)
    node->right = new Node(FACT, NULL, NULL, str);

  return root;
}

void	prepare_rule(std::string& expression, std::string& conclusion)
{
  g_rules.push_back(new Rule(RULE,
			     create_binary_tree_from_expression(expression),
			     create_binary_tree_from_expression(conclusion)));
}
