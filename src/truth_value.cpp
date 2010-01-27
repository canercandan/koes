#include "typedefs.h"
#include "node.h"
#include "sets.h"
#include "functions.h"
#include "globals.h"

RulesSet	g_used_rules;
RulesSet	g_fired_rules;

static bool	find_rule_in_old(Rule* rule)
{
  for (RulesSet::iterator it = g_used_rules.begin(), end = g_used_rules.end();
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

static Rule*	get_a_concluding_rule(Fact F)
{
  for (RulesSet::iterator it = g_rules.begin(), end = g_rules.end();
       it != end; ++it)
    {
      Rule*	rule = *it;
      Node*	conclusion = rule->right;
      if (!find_rule_in_old(rule))
	if (get_fact_from_expression(F, conclusion))
	  {
	    g_used_rules.push_back(rule);
	    return rule;
	  }
    }
  return NULL;
}

static tribool	fire_ability(Rule* R, Fact F)
{
  tribool	condition = bool_expression(R->left);
  if (condition == true)
    {
      if (R->right->op == FACT)
	return true;
      else
	return bool_conclusion(condition, R->right, F);
     }
  return indeterminate;
}

tribool	truth_value(Fact F)
{
  if (g_facts.find(F) != g_facts.end())
    return g_facts[F];
  Rule*		rule;
  tribool	res;
  while ((rule = get_a_concluding_rule(F)) != NULL)
    {
      res = fire_ability(rule, F);
      if (indeterminate(res))
	{
	  g_fired_rules.push_back(rule);
	  continue;
	}
      g_facts[F] = res;
      g_fired_rules.push_back(rule);
      return res;
    }
  return indeterminate;
}
