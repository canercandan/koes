#include <iostream>

#include "functions.h"
#include "globals.h"

void	options_parsing(int ac, char** av)
{
  try
    {
      g_desc.add_options()
	("help,h", "produce help message")
	("filename,f", po::value<StringVector>()->composing(),
	 "provide files")
	("verbose,v", po::value<int>()->default_value(0),
	 "set the verbose (-v 0 = quiet)")
	("wish,w", po::value<StringVector>()->composing(),
	 "provide wishes (facts value)")
	("true,T", po::value<StringVector>()->composing(),
	 "provide a fact to set to true")
	("false,F", po::value<StringVector>()->composing(),
	 "provide a fact to set to false")
	("tree,t", "enable binary tree view while file parsing")
	;

      po::store(po::parse_command_line(ac, av, g_desc), g_vm);
      po::notify(g_vm);

      if (g_vm.count("help"))
	{
	  std::cout << g_desc << std::endl;
	  exit(1);
	}
    }
  catch (std::exception& e)
    {
      std::stringstream ss;
      ss << "error: " << e.what();
      throw std::runtime_error(ss.str());
    }
  catch (...)
    {
      throw std::runtime_error("Exception of unknown type!");
    }
}
