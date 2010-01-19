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
#include "defines.h"

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
  boost::split(vec, expression, boost::is_any_of(EXPR_OPERATORS));

  if (vec.size() == 1) // it means that we have only one fact to set
    facts[expression] = (conclusion == "true") ? true : false;
  else // it means there are several facts to set A&B&C&D->true
    for (int i = 0, size = vec.size(); i < size; ++i)
      facts[vec[i]] = (conclusion == "true") ? true : false;
}

static std::string::size_type	get_operator_position(std::string& expr, std::string::size_type index = 0)
{
  return expr.find_first_of(EXPR_OPERATORS, index);
}

static OperatorEnum	get_operator(Operator& op)
{
  for (int i = 0; operators[i].op != -1; ++i)
    if (operators[i].desc == op)
      return operators[i].op;
  return (OperatorEnum)-1;
}

static void	print_out_binary_tree(Node* node, bool tree = false)
{
  if (node == NULL)
    return;

  if (node->op == FACT)
    {
      if (tree)
	std::cout << "FACT [";
      std::cout << node->data;
      if (tree)
	std::cout << "]" << std::endl;
      return;
    }

  if (tree)
    {
      std::cout << "OP [";
      std::cout << node->data;
      std::cout << "]" << std::endl;
    }

  if (node->left != NULL)
    print_out_binary_tree(node->left, tree);

  if (!tree)
    std::cout << node->data;

  if (node->right != NULL)
    print_out_binary_tree(node->right, tree);
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

static void	fill_out_line(std::string line)
{
  std::vector<std::string>	vec;

  bool	is_fact = line.find("=") != std::string::npos;
  bool	is_rule = line.find("->") != std::string::npos;

  boost::iter_split(vec, line, boost::first_finder(is_fact ? "=" : "->"));

  std::string&	expression = vec[0];
  std::string&	conclusion = vec[1];

  if (is_fact)
    prepare_fact(expression, conclusion);
  else if (is_rule)
    prepare_rule(expression, conclusion);
  else
    throw std::runtime_error("bad delimitor");
}

static void	fill_out(std::ifstream& f)
{
  char	line[1024];
  while (f.getline(line, 1024))
    fill_out_line(line);
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
	("true,T", po::value<StringVector>()->composing(),
	 "provide a fact to set to true")
	("false,F", po::value<StringVector>()->composing(),
	 "provide a fact to set to false")
	;

      po::store(po::parse_command_line(ac, av, desc), vm);
      po::notify(vm);

      if (vm.count("help"))
	{
	  std::cout << desc << std::endl;
	  exit(1);
	}

      if (vm.count("true"))
	{
	  //for (StringVector::iterator it = vm
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

static Node*	get_fact_from_expression(Fact F, Node* rule)
{
  Node*	a;

  if (rule->op == FACT)
    {
      if (rule->data == F)
	return (rule);
      else
	return NULL;
    }
  else
    {
      if ((a = get_fact_from_expression(F, rule->left)) != NULL)
      	return (a);
      if ((a = get_fact_from_expression(F, rule->right)) != NULL)
	return (a);
    }
  return (NULL);
}

static Rule*	get_a_concluding_rule(Fact F, RulesSet& used_rules)
{
  for (RulesSet::iterator it = rules.begin(), end = rules.end();
       it != end; ++it)
    {
      Rule*	rule = *it;
      Node*	conclusion = rule->right;
      if (!find_rule_in_old(rule, used_rules))
	if (get_fact_from_expression(F, conclusion))
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
static int      count_facts_in_expression(Node* node)
{
  int	i;

  i = 0;
  if (node->op == FACT)
    i = 1;
  else
    {
      if (node->op == NOT)
	i = i + count_facts_in_expression(node->right);  
      else
	i = i + count_facts_in_expression(node->left) + count_facts_in_expression(node->right);  
    }
  return i;
    
}
static Boolean	fire_ability(Rule* R)
{
  Boolean	condition;
  //Boolean	conclusion;
  if (count_facts_in_expression(R->right) == 1)
    condition = bool_expression(R->left);
  //else
  //  conclusion = bool_expression(R->right);

  return condition;
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
    sRes = "true";
  else if (res == FALSE)
    sRes = "false";
  else
    sRes = "unknown";
  std::cout << sRes << std::endl;
}

static void	print_out_facts_table()
{
  if (facts.empty())
    {
      std::cout << "no facts into table" << std::endl;
      return;
    }
  std::cout << "print out facts table:" << std::endl;
  for (FactsSet::iterator it = facts.begin(), end = facts.end();
       it != end; ++it)
    std::cout << it->first << " = " << std::boolalpha << it->second << std::endl;
}

static void	print_out_rules_table(bool tree = false)
{
  if (rules.empty())
    {
      std::cout << "no rules into table" << std::endl;
      return;
    }
  std::cout << "print out rules table:" << std::endl;
  for (RulesSet::iterator it = rules.begin(), end = rules.end();
       it != end; ++it)
    {
      if (tree)
	{
	  std::cout << "---" << std::endl;
	  std::cout << "condition:" << std::endl;
	}
      print_out_binary_tree((*it)->left, tree);
      if (tree)
	std::cout << "conclusion:" << std::endl;
      else
	std::cout << " -> ";
      print_out_binary_tree((*it)->right, tree);
      if (!tree)
	std::cout << std::endl;
    }
  if (tree)
    std::cout << "---" << std::endl;
}

static void	files_parsing()
{
  if (!vm.count("filename"))
    return;
  StringVector	fs = vm["filename"].as<StringVector>();
  for (StringVector::iterator it = fs.begin(), end = fs.end();
       it != end; ++it)
    {
      std::cout << "+ filename [" << *it << "]" << std::endl;
      std::ifstream	file(it->c_str());
      fill_out(file);
      file.close();
    }
  print_out_facts_table();
  print_out_rules_table();
}

static void	facts_parsing(bool value)
{
  std::string	sValue(value ? "true" : "false");
  if (!vm.count(sValue))
    return;
  StringVector	sv = vm[sValue].as<StringVector>();
  for (StringVector::iterator it = sv.begin(), end = sv.end();
       it != end; ++it)
    prepare_fact(*it, sValue);
}

int	main(int ac, char** av)
{
  options_parsing(ac, av);
  files_parsing();
  facts_parsing(true);
  facts_parsing(false);

  if (vm.count("wish") && vm.count("filename"))
    {
      StringVector	ws = vm["wish"].as<StringVector>();
      for (StringVector::iterator it = ws.begin(), end = ws.end();
	   it != end; ++it)
	print_result(*it);
    }
  else
    {
      std::cout << "CTRL+C to quit or type ? for help" << std::endl;
      while (1)
	{
	  std::string	cmd;
	  std::cout << "es> ";
	  std::cin >> cmd;

	  if (cmd == "?" || cmd == "help")
	    std::cout << "Available commands:" << std::endl
		      << "- ? or help\t\t: To print help" << std::endl
		      << "- ?F\t\t\t: To print facts table" << std::endl
		      << "- ?R\t\t\t: To print rules table" << std::endl
		      << "- ?RR\t\t\t: To print rules table with the binary tree" << std::endl
		      << "- FACT=VALUE\t\t: To set VALUE to FACT" << std::endl
		      << "- CONDITION->CONCLUSION\t: To create a new rule" << std::endl;
	  else if (cmd == "?F")
	    print_out_facts_table();
	  else if (cmd == "?R")
	    print_out_rules_table();
	  else if (cmd == "?RR")
	    print_out_rules_table(true);
	  else if (cmd.find("=") != std::string::npos ||
		   cmd.find("->") != std::string::npos)
	    fill_out_line(cmd);
	  else
	    print_result(cmd);
	}
    }

  delete_rules();

  return 0;
}
