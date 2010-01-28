#include <iostream>
#include "functions.h"
#include "globals.h"
#include "sets.h"
StringVector	fired_facts;

static bool	fact_is_in_fired_facts(Fact fact)
{
  for (StringVector::iterator it = fired_facts.begin(), end = fired_facts.end();
       it != end; ++it)
    {
      if ((*it) == fact)
	return true;
    }
  return false;
}


static bool	fact_is_in_g_facts(Fact fact)
{
  if (g_facts.find(fact) != g_facts.end())
    return true;
  return false;
}

static void	get_facts_from_node(Node *node)
{
  if (node->op == FACT)
    {
      if (!fact_is_in_fired_facts(node->data) && !fact_is_in_g_facts(node->data))
	fired_facts.push_back(node->data);
    }
  else
    {
      get_facts_from_node(node->left);
      get_facts_from_node(node->right);
    }
  return ;
}

static void	get_facts_from_fired_rules()
{
  for (RulesSet::reverse_iterator it = g_fired_rules.rbegin(), end = g_fired_rules.rend();
       it != end; ++it)
    {
      get_facts_from_node((*it)->left);
      get_facts_from_node((*it)->right);
    }
  return ;
}

static void		ask_question()
{
  get_facts_from_fired_rules();
  for (StringVector::iterator it = fired_facts.begin(), end = fired_facts.end();
       it != end; ++it)
    {
      std::string	b_str;
      if (*it == "")
	continue;
      std::cout << "what is the fact?[true|false]" << std::endl;
      std::cout << "? " << *it << "=";
      std::cin >> b_str;
      if (b_str == "true")
	g_facts[*it] = true;
      else if (b_str == "false")
	g_facts[*it] = false;
      break;
    }
  fired_facts.clear();
  g_used_rules.clear();
  g_fired_rules.clear();
}

void	print_result(Fact F)
{
  tribool	res;
 
  while (indeterminate(res = truth_value(F)))
    ask_question();
  std::cout << "Searching for " << F << " = " << bool_to_string(res) << std::endl;
  if (g_vm.count("verbose") && g_vm["verbose"].as<int>() >= 1)
    {
      print_out_fired_rules();     
      print_out_facts_table();
    }
  g_facts = g_initial_facts;
}
