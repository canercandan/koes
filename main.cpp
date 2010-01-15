#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

typedef std::string	Data;
typedef Data		Fact;
typedef Data		Operator;

static Operator	operators[] = {"&", "|", "^", "!", "->"};

//struct Entry;
struct Entry
{
  Data		data;
  Entry*	left;
  Entry*	right;
};

typedef std::vector< Entry > RulesSet;
typedef std::map< Fact, bool > FactsSet;

static RulesSet	rules;
static FactsSet	facts;

static void fill_out(std::ifstream& f)
{
  char	line[1024];
  while (f.getline(line, 1024))
    {
      std::vector<std::string>	vec;
      boost::iter_split(vec, line, boost::first_finder("->"));
      std::copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(std::cout, " "));
      std::cout << std::endl;

      std::string&	expression = vec[0];
      std::string&	conclusion = vec[1];

      if (conclusion == "true" || conclusion == "false")
	{
	  std::vector<std::string>	v;
	  boost::split(v, expression, boost::is_any_of("&|^"));

	  if (v.size() == 1) // it means that we have only one fact to set
	    {
	      facts[expression] = (conclusion == "true") ? true : false;
	    }
	  else // it means there are several facts to set A&B&C&D->true
	    {
	      for (int i = 0, size = v.size(); i < size; ++i)
		facts[v[i]] = (conclusion == "true") ? true : false;
	    }
	}
    }
}

int	main(int ac, char** av)
{
  if (ac < 2)
    throw std::runtime_error("no argument");

  for (int i = 1; i < ac; ++i)
    {
      std::ifstream f;
      f.open(av[i]);
      fill_out(f);
      f.close();
    }

  std::cout << "print out facts table:" << std::endl;
  for (FactsSet::iterator it = facts.begin(), end = facts.end();
       it != end; ++it)
    {
      std::cout << it->first << " = " << std::boolalpha << it->second << std::endl;
    }
}
