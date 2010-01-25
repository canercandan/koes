#include <stdexcept>
#include "functions.h"

static tribool	and_operation(tribool a, tribool b);
static tribool	or_operation(tribool a, tribool b);
static tribool	xor_operation(tribool a, tribool b);
static tribool	not_operation(tribool a, tribool b);

typedef tribool (*op_func)(tribool, tribool);

static op_func	operationArray[] = {
  and_operation,
  or_operation,
  xor_operation,
  not_operation
};

tribool	operations(OperatorEnum op, tribool a, tribool b)
{
  if (!(op >= AND && op <= NOT))
    throw std::runtime_error("bad operator");
  return (operationArray[op - 2])(a, b);
}

static tribool	and_operation(tribool a, tribool b)
{
  if (a == true && b == true)
    return (true);
  if (a == false || b == false)
    return (false);
  return (indeterminate);
}

static tribool	or_operation(tribool a, tribool b)
{
  if (a == true || b == true)
    return (true);
  if (a == false && b == false)
    return (false);
  return (indeterminate);
}

static tribool	xor_operation(tribool a, tribool b)
{
  if (a == true || b == true)
    return (false);
  if (a == false && b == false)
    return (true);
  return (indeterminate);
}

static tribool	not_operation(tribool a, tribool b)
{
  (void)a;
  if (b == true)
    return (false);
  if (b == false)
    return (true);
  return (indeterminate);
}
