#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/program_options.hpp>

#include "typedefs.h"
#include "node.h"
#include "sets.h"

static Boolean	xor_operation(Boolean a, Boolean b);
static Boolean	or_operation(Boolean a, Boolean b);
static Boolean	and_operation(Boolean a, Boolean b);
static Boolean	not_operation(Boolean a, Boolean b);

op_func	operationArray[] = {
  and_operation,
  or_operation,
  xor_operation,
  not_operation
};

static Boolean	operation(OperatorEnum op, Boolean a, Boolean b);

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

namespace po = boost::program_options;

static po::options_description	desc;
static po::variables_map	vm;

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
    delete left;
  if (right != NULL)
    delete right;
}

static void	prepare_fact(std::string& expression, std::string& conclusion)
{
  std::vector<std::string>	vec;
  boost::split(vec, expression, boost::is_any_of("&|^!"));

  if (vec.size() == 1) // it means that we have only one fact to set
    facts[expression] = (conclusion == "true") ? true : false;
  else // it means there are several facts to set A&B&C&D->true
    for (int i = 0, size = vec.size(); i < size; ++i)
      facts[vec[i]] = (conclusion == "true") ? true : false;
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
  std::string	delim;
  std::string	s;

  while (f.getline(line, 1024))
    {
      std::vector<std::string>	vec;
      s = line;
      (s.find("=") != std::string::npos) ? (delim = "=") : (delim = "->");
      boost::iter_split(vec, line, boost::first_finder(delim));

      std::string&	expression = vec[0];
      std::string&	conclusion = vec[1];

      if (conclusion == "true" || conclusion == "false") // it's a fact
	prepare_fact(expression, conclusion);
      else // it's a rule
	prepare_rule(expression, conclusion);
    }
}

static void	options_parsing(int ac, char** av)
{
  try
    {
      desc.add_options()
	("help,h", "produce help message")
	("filename,f", po::value<StringVector>()->composing(),
	 "provide files")
	("verbose,v", po::value<int>()->default_value(0),
	 "set the verbose (-v 0 = quiet)")
	("wish,w", po::value<StringVector>()->composing(),
	 "provide wishes (facts value)")
	;

      po::store(po::parse_command_line(ac, av, desc), vm);
      po::notify(vm);

      if (vm.count("help") || !vm.count("filename"))
	{
	  std::cout << desc << std::endl;
	  exit(1);
	}
    }
  catch (std::exception& e)
    {
      std::stringstream ss;
      ss << "error: " << e.what();
      throw std::runtime_error(ss.str());
    }
  catch (...)
    {
      throw std::runtime_error("Exception of unknown type!");
    }
}

static bool	find_rule_in_old(Rule* rule, RulesSet& used_rules)
{
  for (RulesSet::iterator it = used_rules.begin(), end = used_rules.end();
       it != end; ++it)
    {
      if (rule == *it)
	return (true);
    }
  return (false);
}

static Rule*	get_a_concluding_rule(Fact F, RulesSet& used_rules)
{
  for (RulesSet::iterator it = rules.begin(), end = rules.end();
       it != end; ++it)
    {
      Rule*	rule = *it;
      Node*	conclusion = rule->right;
      if (!find_rule_in_old(rule, used_rules))
	if (conclusion->op == FACT)
	  if (conclusion->data == F)
	    {
	      used_rules.push_back(rule);
	      return rule;
	    }
    }
  return NULL;
}

static Boolean	bool_expression(Node* exp);
static Boolean	truth_value(Fact);
static Boolean	fire_ability(Rule*);

static Boolean	fire_ability(Rule* R)
{
  return bool_expression(R->left);
}

static Boolean	truth_value(Fact F)
{
  Rule*		rule;
  RulesSet	used_rules;

  if (facts.find(F) != facts.end() && facts.find(F)->second == true)
    return TRUE;
  if (facts.find(F) != facts.end() && facts.find(F)->second == false)
    return FALSE;
  while ((rule = get_a_concluding_rule(F, used_rules)) != NULL)
    {
      if (fire_ability(rule) == 1)
	{
	  facts[F] = true;
	  return TRUE;
	}
      else if (fire_ability(rule) == 0)
	{
	  facts[F] = false;
	  return FALSE;
	}
    }
  used_rules.clear();
  return UNKNOWN;
}

static Boolean	bool_expression(Node* exp)
{
  if (exp->op == FACT)
    return truth_value(exp->data);

  return operation(exp->op, truth_value(exp->left->data),
		   (exp->right->op == FACT) ?
		   truth_value(exp->right->data) :
		   bool_expression(exp->right));
}

static Boolean	operation(OperatorEnum op, Boolean a, Boolean b)
{
  if (!(op >= AND && op <= NOT))
    throw std::runtime_error("bad operator");
  return (operationArray[op - 2])(a, b);
}

static Boolean	and_operation(Boolean a, Boolean b)
{
  if (a == TRUE && b == TRUE)
    return (TRUE);
  if (a == FALSE || b == FALSE)
    return (FALSE);
  return (UNKNOWN);
}

static Boolean	or_operation(Boolean a, Boolean b)
{
  if (a == TRUE || b == TRUE)
    return (TRUE);
  if (a == FALSE && b == FALSE)
    return (FALSE);
  return (UNKNOWN);
}

static Boolean	xor_operation(Boolean a, Boolean b)
{
  if (a == TRUE || b == TRUE)
    return (FALSE);
  if (a == FALSE && b == FALSE)
    return (TRUE);
  return (UNKNOWN);
}

static Boolean	not_operation(Boolean a, Boolean b)
{
  (void)a;
  if (b == TRUE)
    return (FALSE);
  if (b == FALSE)
    return (TRUE);
  return (UNKNOWN);
}

static void	print_result(Fact F)
{
  Boolean	res = truth_value(F);
  std::string	sRes;

  if (res == TRUE)
    sRes = "TRUE";
  else if (res == FALSE)
    sRes = "FALSE";
  else
    sRes = "UNKNOWN";

  std::cout << "[" << F << "] = [" << sRes << "]" << std::endl;
}

int	main(int ac, char** av)
{
  options_parsing(ac, av);

  StringVector	fs = vm["filename"].as<StringVector>();
  for (StringVector::iterator it = fs.begin(), end = fs.end();
       it != end; ++it)
    {
      std::cout << "FILENAME [" << *it << "]" << std::endl;
      std::ifstream f(it->c_str());
      fill_out(f);
      f.close();
    }

  std::cout << "print out facts table:" << std::endl;
  for (FactsSet::iterator it = facts.begin(), end = facts.end();
       it != end; ++it)
    std::cout << it->first << " = " << std::boolalpha << it->second << std::endl;

  std::cout << "print out rules table:" << std::endl;
  for (RulesSet::iterator it = rules.begin(), end = rules.end();
       it != end; ++it)
    print_out_binary_tree(*it);

  if (vm.count("wish"))
    {
      StringVector	ws = vm["wish"].as<StringVector>();
      for (StringVector::iterator it = ws.begin(), end = ws.end();
	   it != end; ++it)
	print_result(*it);
    }
  else
    {
      std::string a;
      std::cin >> a;
      print_result(a);
    }

  delete_rules();

  return 0;
}
