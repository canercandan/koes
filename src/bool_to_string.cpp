#include "functions.h"

std::string	bool_to_string(tribool b)
{
  if (b)
    return ("true");
  if (!b)
    return ("false");
  return ("unknown");
}
