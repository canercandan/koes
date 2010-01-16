#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

#include "typedefs.h"
#include "node.h"
#include "sets.h"

static OperatorStruct	operators[] = {
  {FACT, "F"},
  {RULE, "->"},
  {AND, "&"},
  {OR, "|"},
  {XOR, "^"},
  {NOT, "!"},
  {(OperatorEnum)-1, ""}
};

static RulesSet	rules;
static FactsSet	facts;

Node::Node(OperatorEnum _op /*= FACT*/,
	   Node* _left /*= NULL*/,
	   Node* _right /*= NULL*/,
	   Data _data /*= "NONE"*/)
  : op(_op), left(_left), right(_right), data(_data)
{
  if (data == "NONE")
    data = operators[op].desc;
}

Node::~Node()
{
  if (left != NULL)
    {
      delete left;
      left = NULL;
    }
  if (right != NULL)
    {
      delete right;
      right = NULL;
    }
}

static void	prepare_fact(std::string& expression, std::string& conclusion)
{
  std::vector<std::string>	vec;
  boost::split(vec, expression, boost::is_any_of("&|^!"));

  if (vec.size() == 1) // it means that we have only one fact to set
    {
      facts[expression] = (conclusion == "true") ? true : false;
    }
  else // it means there are several facts to set A&B&C&D->true
    {
      for (int i = 0, size = vec.size(); i < size; ++i)
	facts[vec[i]] = (conclusion == "true") ? true : false;
    }
}

static std::string::size_type	get_operator_position(std::string& expr, std::string::size_type index = 0)
{
  return expr.find_first_of("&|^!", index);
}

static OperatorEnum	get_operator(Operator& op)
{
  for (int i = 0; operators[i].op != -1; ++i)
    if (operators[i].desc == op)
      return operators[i].op;
  return (OperatorEnum)-1;
}

static void	print_out_binary_tree(Node* node)
{
  if (node == NULL)
    return;

  if (node->op == FACT)
    {
      std::cout << "FACT [" << node->data << "]" << std::endl;
      return;
    }

  std::cout << "OP [" << node->data << "]" << std::endl;

  if (node->left != NULL)
    print_out_binary_tree(node->left);

  if (node->right != NULL)
    print_out_binary_tree(node->right);
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

  print_out_binary_tree(root);

  return root;
}

static void	prepare_rule(std::string& expression, std::string& conclusion)
{
  rules.push_back(new Rule(RULE,
			   create_binary_tree_from_expression(expression),
			   create_binary_tree_from_expression(conclusion)));
}

static void	delete_rules()
{
  for (int i = 0, size = rules.size(); i < size; ++i)
    delete rules[i];
}

static void	fill_out(std::ifstream& f)
{
  char	line[1024];
  while (f.getline(line, 1024))
    {
      std::vector<std::string>	vec;
      boost::iter_split(vec, line, boost::first_finder("->"));
      std::copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(std::cout, "->"));
      std::cout << std::endl;

      std::string&	expression = vec[0];
      std::string&	conclusion = vec[1];

      if (conclusion == "true" || conclusion == "false") // it's a fact
	prepare_fact(expression, conclusion);
      else // it's a rule
	prepare_rule(expression, conclusion);
    }
}

int	main(int ac, char** av)
{
  if (ac < 2)
    throw std::runtime_error("no argument");

  for (int i = 1; i < ac; ++i)
    {
      std::ifstream f;
      f.open(av[i]);
      fill_out(f);
      f.close();
    }

  std::cout << "print out facts table:" << std::endl;
  for (FactsSet::iterator it = facts.begin(), end = facts.end();
       it != end; ++it)
    {
      std::cout << it->first << " = " << std::boolalpha << it->second << std::endl;
    }
  //  printf("%d\n", facts["A"]);
bool b = truth_value("C", rules, facts);
 std::cout << b << std::endl;
  delete_rules();
}
