#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "typedefs.h"
#include "node.h"
#include "sets.h"
#include "functions.h"
#include "globals.h"


RulesSet	g_rules;
FactsSet	g_facts;

namespace po = boost::program_options;

po::variables_map	g_vm;
po::options_description	g_desc;

static void	delete_rules()
{
  for (int i = 0, size = g_rules.size(); i < size; ++i)
    delete g_rules[i];
}

static void	print_result(Fact F)
{
  Boolean	res = truth_value(F);
  std::string	sRes;

  if (res == TRUE)
    sRes = "true";
  else if (res == FALSE)
    sRes = "false";
  else
    sRes = "unknown";
  std::cout << sRes << std::endl;
}

int	main(int ac, char** av)
{
  options_parsing(ac, av);
  files_parsing();
  facts_parsing(true);
  facts_parsing(false);

  if (g_vm.count("wish") && g_vm.count("filename"))
    {
      StringVector	ws = g_vm["wish"].as<StringVector>();
      for (StringVector::iterator it = ws.begin(), end = ws.end();
	   it != end; ++it)
	print_result(*it);
    }
  else
    {
      std::cout << "CTRL+C to quit or type ? for help" << std::endl;
      while (1)
	{
	  std::string	cmd;
	  std::cout << "es> ";
	  std::cin >> cmd;

	  if (cmd == "?" || cmd == "help")
	    std::cout << "Available commands:" << std::endl
		      << "- ? or help\t\t: To print help" << std::endl
		      << "- ?F\t\t\t: To print facts table" << std::endl
		      << "- ?R\t\t\t: To print rules table" << std::endl
		      << "- ?RR\t\t\t: To print rules table with the binary tree" << std::endl
		      << "- FACT=VALUE\t\t: To set VALUE to FACT" << std::endl
		      << "- CONDITION->CONCLUSION\t: To create a new rule" << std::endl;
	  else if (cmd == "?F")
	    print_out_facts_table();
	  else if (cmd == "?R")
	    print_out_rules_table();
	  else if (cmd == "?RR")
	    print_out_rules_table(true);
	  else if (cmd.find("=") != std::string::npos ||
		   cmd.find("->") != std::string::npos)
	    fill_out_line(cmd);
	  else
	    print_result(cmd);
	  used_rules.clear();
	}
    }

  delete_rules();
  return 0;
}
