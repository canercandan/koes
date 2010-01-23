#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

#include "functions.h"
#include "globals.h"

void	fill_out_line(std::string line)
{
  if (line.size() <= 0)
    return;

  std::vector<std::string>	vec;

  bool	is_fact = line.find("=") != std::string::npos;
  bool	is_rule = line.find("->") != std::string::npos;

  boost::iter_split(vec, line, boost::first_finder(is_fact ? "=" : "->"));

  std::string&	expression = vec[0];
  std::string&	conclusion = vec[1];

  if (is_fact)
    prepare_fact(expression, conclusion);
  else if (is_rule)
    prepare_rule(expression, conclusion);
  else
    throw std::runtime_error("bad delimitor");
}
