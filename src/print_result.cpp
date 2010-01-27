#include <iostream>
#include "functions.h"
#include "globals.h"

void	print_result(Fact F)
{
  tribool	res = truth_value(F);
  std::cout << "Search for " << F << " = " << bool_to_string(res) << std::endl;
  print_out_fired_rules();
  g_facts = g_initial_facts;
}
