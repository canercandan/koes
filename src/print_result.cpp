#include <iostream>
#include "functions.h"

void	print_result(Fact F)
{
  tribool	res = truth_value(F);
  std::cout << bool_to_string(res) << std::endl;
  print_out_fired_rules();
}
