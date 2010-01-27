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
FactsSet	g_initial_facts;
StringVector	g_wishes;

namespace po = boost::program_options;

po::variables_map	g_vm;
po::options_description	g_desc;

static void	delete_rules()
{
  for (int i = 0, size = g_rules.size(); i < size; ++i)
    delete g_rules[i];
}

static void	print_command_usage()
{
  std::cout << "Available commands:" << std::endl
	    << "- ? or help\t\t: To print help" << std::endl
	    << "- ?F\t\t\t: To print facts table" << std::endl
	    << "- ?R\t\t\t: To print rules table" << std::endl
	    << "- ?RR\t\t\t: To print rules table with the binary tree" << std::endl
	    << "- FACT=VALUE\t\t: To set VALUE to FACT" << std::endl
	    << "- CONDITION->CONCLUSION\t: To create a new rule" << std::endl;

}

static void	parse_command(std::string& cmd)
{
  if (cmd == "?" || cmd == "help")
    print_command_usage();
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
}

int	main(int ac, char** av)
{
  options_parsing(ac, av);
  files_parsing();
  facts_parsing(true);
  facts_parsing(false);

  //print_out_facts_table();

  if (g_vm.count("wish") && g_vm.count("filename"))
    {
      StringVector	ws = g_vm["wish"].as<StringVector>();
      // g_wishes += ws;
      for (StringVector::iterator it = ws.begin(), end = ws.end();
      	   it != end; ++it)
      	g_wishes.push_back(*it);
    }

  if (g_wishes.size() > 0 && g_vm.count("filename"))
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
	  parse_command(cmd);
	  g_used_rules.clear();
	  g_fired_rules.clear();
	}
    }

  delete_rules();
  return 0;
}
