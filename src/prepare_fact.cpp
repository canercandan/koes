#include <stdexcept>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

#include "functions.h"
#include "globals.h"
#include "defines.h"

void	prepare_fact(std::string& expression, std::string& conclusion)
{
  std::vector<std::string>	vec;
  try
    {
      boost::split(vec, expression, boost::is_any_of(EXPR_OPERATORS));
    }
  catch (...)
    {
      throw std::runtime_error(std::string("expression:" + expression));
    }

  if (vec.size() == 1) // it means that we have only one fact to set
    g_initial_facts[expression] = (conclusion == "true") ? true : false;
  else // it means there are several facts to set A&B&C&D->true
    for (int i = 0, size = vec.size(); i < size; ++i)
      g_initial_facts[vec[i]] = (conclusion == "true") ? true : false;
}
