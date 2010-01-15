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
  {FLAG, "F"},
  {RULE, "->"},
  {AND, "&"},
  {OR, "|"},
  {XOR, "^"},
  {NOT, "!"},
  {(OperatorEnum)-1, ""}
};

static RulesSet	rules;
static FactsSet	facts;

Node::Node(OperatorEnum _op /*= FLAG*/,
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
  std::vector<std::string>	v;
  boost::split(v, expression, boost::is_any_of("&|^!"));

  if (v.size() == 1) // it means that we have only one fact to set
    {
      facts[expression] = (conclusion == "true") ? true : false;
    }
  else // it means there are several facts to set A&B&C&D->true
    {
      for (int i = 0, size = v.size(); i < size; ++i)
	facts[v[i]] = (conclusion == "true") ? true : false;
    }
}

static Node*	create_binary_tree_from_expression(std::string& expr)
{
  (void)expr;

  

  return new Node();
}

static void	prepare_rule(std::string& expression, std::string& conclusion)
{
  Node*		left = create_binary_tree_from_expression(expression);
  Node*		right = create_binary_tree_from_expression(conclusion);

  (void)left;
  (void)right;

  rules.push_back(Rule(RULE,
		       new Node(FLAG, NULL, NULL, expression),
		       new Node(FLAG, NULL, NULL, conclusion)));
}

static void	fill_out(std::ifstream& f)
{
  char	line[1024];
  while (f.getline(line, 1024))
    {
      std::vector<std::string>	vec;
      boost::iter_split(vec, line, boost::first_finder("->"));
      std::copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(std::cout, " "));
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
}
