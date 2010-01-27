#include <iostream>
#include "functions.h"
#include "globals.h"

void	print_result(Fact F)
{
  tribool	res = truth_value(F);
  //  while (res == unknown)
  // question_parsing();
  std::cout << "Searching for " << F << " = " << bool_to_string(res) << std::endl;
  if (g_vm.count("verbose") && g_vm["verbose"].as<int>() >= 1)
    {
      print_out_fired_rules();
      print_out_facts_table();
    }
  g_facts = g_initial_facts;
}
