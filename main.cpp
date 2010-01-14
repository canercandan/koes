#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

typedef char	Fact;

enum Operator { AND, OR, XOR, NOT, SEPERATOR };

union Data
{
  Fact		fact;
  Operator	op;
};

//struct Entry;
struct Entry
{
  Data	data;
  Entry* left;
  Entry* right;
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
	  if (expression.size() == 1) // it means that we have only one fact to set
	    {
	      Fact fact = expression[0];
	      facts[fact] = (conclusion == "true") ? true : false;
	    }
	  else // it means there are several facts to set A&B&C&D->true
	    {
	    }
	}
    }
}

int	main(int ac, char** av)
{
  std::ifstream f;
  f.open("file.txt");
  fill_out(f);
  f.close();

  std::cout << "print out facts table:" << std::endl;
  for (FactsSet::iterator it = facts.begin(), end = facts.end();
       it != end; ++it)
    {
      std::cout << it->first << " = " << std::boolalpha << it->second << std::endl;
    }
}
