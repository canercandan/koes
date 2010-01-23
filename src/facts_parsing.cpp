#include "functions.h"
#include "globals.h"

void	facts_parsing(bool value)
{
  std::string	sValue(value ? "true" : "false");
  if (!g_vm.count(sValue))
    return;
  StringVector	sv = g_vm[sValue].as<StringVector>();
  for (StringVector::iterator it = sv.begin(), end = sv.end();
       it != end; ++it)
    prepare_fact(*it, sValue);
}
