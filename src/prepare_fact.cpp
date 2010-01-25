#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

#include "functions.h"
#include "globals.h"
#include "defines.h"

void	prepare_fact(std::string& expression, std::string& conclusion)
{
  std::vector<std::string>	vec;
  boost::split(vec, expression, boost::is_any_of(EXPR_OPERATORS));

  if (vec.size() == 1) // it means that we have only one fact to set
    g_facts[expression] = (conclusion == "true") ? true : false;
  else // it means there are several facts to set A&B&C&D->true
    for (int i = 0, size = vec.size(); i < size; ++i)
      g_facts[vec[i]] = (conclusion == "true") ? true : false;
}
