#include <iostream>

#include "functions.h"
#include "globals.h"

void	print_out_facts_table()
{
  if (g_facts.empty())
    {
      std::cout << "no facts into table" << std::endl;
      return;
    }
  std::cout << "print out facts table:" << std::endl;
  for (FactsSet::iterator it = g_facts.begin(), end = g_facts.end();
       it != end; ++it)
    std::cout << it->first << " = " << bool_to_string(it->second) << std::endl;
}
