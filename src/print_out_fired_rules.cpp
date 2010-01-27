#include <iostream>

#include "functions.h"
#include "globals.h"

void	print_out_fired_rules()
{
  if (g_fired_rules.empty())
    {
      std::cout << "no rules been fired" << std::endl;
      return;
    }
  std::cout << "print out fired rules:" << std::endl;
  for (RulesSet::iterator it = g_fired_rules.begin(), end = g_fired_rules.end();
       it != end; ++it)
    {
      print_out_binary_tree((*it)->left);
      std::cout << "-> ";
      print_out_binary_tree((*it)->right);
      std::cout << std::endl;
    }
}
