#include <iostream>
#include <fstream>

#include "functions.h"
#include "globals.h"

static void	fill_out(std::ifstream& f)
{
  char	line[1024];
  while (f.getline(line, 1024))
    fill_out_line(line);
}

void	files_parsing()
{
  if (!g_vm.count("filename"))
    return;
  StringVector	fs = g_vm["filename"].as<StringVector>();
  for (StringVector::iterator it = fs.begin(), end = fs.end();
       it != end; ++it)
    {
      std::cout << "+ filename [" << *it << "]" << std::endl;
      std::ifstream	file(it->c_str());
      fill_out(file);
      file.close();
    }
  g_facts = g_initial_facts;
  print_out_facts_table();
  print_out_rules_table(g_vm.count("tree"));
}
