#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "typedefs.h"
#include "node.h"
#include "sets.h"
#include "functions.h"
#include "globals.h"

RulesSet	g_rules;
FactsSet	g_facts;

po::options_description	g_desc;
po::variables_map	g_vm;

static void	delete_rules()
{
  for (int i = 0, size = g_rules.size(); i < size; ++i)
    delete g_rules[i];
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
  for (RulesSet::iterator it = g_rules.begin(), end = g_rules.end();
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
static Boolean	fire_ability(Rule*, Fact);

static Boolean	fire_ability(Rule* R, Fact F)
{
  Boolean	condition;
  Fact left;
  Fact right;
 
  condition = bool_expression(R->left);
  if (condition == TRUE)
     {
      if (R->right->op == FACT)
	return TRUE;
      else		       
 	{
	  if (R->right->op == NOT)
	    {
	      return FALSE;
	    }
	  else if (R->right->op == AND)
	    {
	      return TRUE;
	    }
	  else if (R->right->op == OR)
	    {
	      left = R->right->left->data;
	      right = R->right->right->data;
	      if ((left != F && truth_value(left) == FALSE)
		  || (right != F && truth_value(right) == FALSE))
		return TRUE;
	    }
	  else
 	    return FALSE;
 	}
     }
  return UNKNOWN;
}

static Boolean	truth_value(Fact F)
{
  Rule*		rule;
  RulesSet	used_rules;

  if (g_facts.find(F) != g_facts.end())
    return g_facts[F];
  while ((rule = get_a_concluding_rule(F, used_rules)) != NULL)
    {
      if (fire_ability(rule, F) == TRUE)
	{
	  g_facts[F] = TRUE;
	  return TRUE;
	}
      if (fire_ability(rule, F) == FALSE)
	{
	  g_facts[F] = FALSE;
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

  return operations(exp->op, truth_value(exp->left->data),
		    (exp->right->op == FACT) ?
		    truth_value(exp->right->data) :
		    bool_expression(exp->right));
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

int	main(int ac, char** av)
{
  options_parsing(ac, av);
  files_parsing();
  facts_parsing(true);
  facts_parsing(false);

  if (g_vm.count("wish") && g_vm.count("filename"))
    {
      StringVector	ws = g_vm["wish"].as<StringVector>();
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
