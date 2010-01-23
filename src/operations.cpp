#include <stdexcept>
#include "functions.h"

static Boolean	and_operation(Boolean a, Boolean b);
static Boolean	or_operation(Boolean a, Boolean b);
static Boolean	xor_operation(Boolean a, Boolean b);
static Boolean	not_operation(Boolean a, Boolean b);

static op_func	operationArray[] = {
  and_operation,
  or_operation,
  xor_operation,
  not_operation
};

Boolean	operations(OperatorEnum op, Boolean a, Boolean b)
{
  if (!(op >= AND && op <= NOT))
    throw std::runtime_error("bad operator");
  return (operationArray[op - 2])(a, b);
}

static Boolean	and_operation(Boolean a, Boolean b)
{
  if (a == TRUE && b == TRUE)
    return (TRUE);
  if (a == FALSE || b == FALSE)
    return (FALSE);
  return (UNKNOWN);
}

static Boolean	or_operation(Boolean a, Boolean b)
{
  if (a == TRUE || b == TRUE)
    return (TRUE);
  if (a == FALSE && b == FALSE)
    return (FALSE);
  return (UNKNOWN);
}

static Boolean	xor_operation(Boolean a, Boolean b)
{
  if (a == TRUE || b == TRUE)
    return (FALSE);
  if (a == FALSE && b == FALSE)
    return (TRUE);
  return (UNKNOWN);
}

static Boolean	not_operation(Boolean a, Boolean b)
{
  (void)a;
  if (b == TRUE)
    return (FALSE);
  if (b == FALSE)
    return (TRUE);
  return (UNKNOWN);
}
