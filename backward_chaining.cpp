#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include "typedefs.h"
#include "sets.h"
#include "node.h"

static void	condition(std::vector<Fact>& f_condition, Rule& R)
{
  while (R != NULL)
    {
      if (R->op == FACT)
	f_condition.push_back(R->data);
      condition(f_condition, R->left);
      condition(f_condition, R->right);
    }
  return ;
}

static bool		there_is_a_concluding_rule(RulesSet rules, Fact F)
{
  for (RulesSet::iterator it = rules.begin(), end = rules.end(); it != end; ++it)
    {
      if (*it->data == F)
	return (true);
    }
  return (false);
}

static Rule*	select_a_concluding_rule(RulesSet& rules, Fact F)
{
  Rule*	r;

  for (RulesSet::iterator it = rules.begin(), end = rules.end(); it != end; ++it)
    {
      if (*it->data == F)
	r = *it;
    }
  return (r);
}

static bool	fire_ability(Rule &R, RulesSet &rules, FactsSet &facts)
{
  std::vector<Fact>	facts_to_check;
  Fact			F;

  condition(facts_to_check, R);
  for (vector::iterator it = facts_to_check.begin(), end = facts_to_check.end(); it != end; ++it)
    {
      F = *it;
      if (truth_value(F, rules, facts) == false)
	return (false);
      facts_to_check.earse(F);
    } 
  return (true);
}

static bool	truth_value(Fact F, RulesSet &rules, FactsSet &facts)
{
  Rule r;

  if (facts.find(F) != map::end)
    return (facts[F]);
  else
    {
      while (there_is_a_concluding_rule(rules, F))
	{
	  r = select_a_concluding_rule(rules, F);
	  rules.erase(r);
	  if (fire_ability(r, rules, facts) == true)
	    {
	      facts[F] = "true";
	      return (true);
	    }
	}
      return (false);
    }
}

