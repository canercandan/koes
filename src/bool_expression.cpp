#include "typedefs.h"
#include "node.h"
#include "functions.h"

tribool	bool_expression(Node* exp)
{
  if (exp->op == FACT)
    return truth_value(exp->data);

  return operations(exp->op, truth_value(exp->left->data),
		    (exp->right->op == FACT) ?
		    truth_value(exp->right->data) :
		    bool_expression(exp->right));
}
