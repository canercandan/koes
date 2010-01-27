#include <iostream>
#include <stdexcept>

#include "functions.h"
#include "node.h"
#include "typedefs.h"
#include "globals.h"

typedef tribool (*op_exp)(tribool, Node*, Fact);

static tribool	and_expression(tribool, Node*, Fact);
static tribool	or_expression(tribool, Node*, Fact);
static tribool	xor_expression(tribool, Node*, Fact);
static tribool	not_expression(tribool, Node*, Fact);

static op_exp	expressionArray[] = {
  and_expression,
  or_expression,
  xor_expression,
  not_expression
};

tribool	bool_conclusion(tribool condition, Node* exp, Fact F)
{
  if (!(exp->op >= AND && exp->op <= NOT))
    throw std::runtime_error("bad operator");
  return (expressionArray[exp->op - 2])(condition, exp, F);
}

static tribool	and_expression(tribool condition, Node* exp, Fact F)
{
  Node*	exp_left = exp->left;
  Node*	exp_right = exp->right;

  if (condition == true)
    {
      if (exp_right->op == FACT)
	{
	  (exp_right->data == F) ? (g_facts[exp_left->data] = true)
	    : (g_facts[exp_right->data] = true);
	  return true;
	}
      else
	{
	  if (exp_left->data == F)
	    return true;
	  else
	    return bool_conclusion(true, exp->right, F);
	}
    }
  else if (condition == false)
    {
      if (exp_right->op == FACT)
	{
	  if (exp_left->data == F && truth_value(exp_right->data) == true)
	    return false;
	}
      else
	{
	  if (exp_left->data == F && bool_expression(exp_right) == true)
	    return false;
	  else if (truth_value(exp_left->data) == true)
	    return bool_conclusion(false, exp->right, F);
	}
    }
  return indeterminate;
}

static tribool	or_expression(tribool condition, Node* exp, Fact F)
{
  Node*	exp_left = exp->left;
  Node* exp_right = exp->right;
  if (condition == true)
    {
      if (exp_right->op == FACT)
	{
	  if ((exp_left->data != F && truth_value(exp_left->data) == false)
	      || (exp_right->data != F && truth_value(exp_right->data) == false))
	    return true;
	}
      else
	{
	  if (exp_left->data == F && bool_expression(exp_right) == false)
	    return true;
	  else if (truth_value(exp_left->data) == false)
	    return bool_conclusion(true, exp_right, F);
	}
    }
  else if (condition == false)
    {
      if (exp_right->op == FACT)
	{
	  if ((exp_left->data != F && truth_value(exp_left->data) == true)
	      || (exp_right->data != F && truth_value(exp_right->data) == true))
 	    return false;
	}
      else
	{
	  if (exp_left->data == F && bool_expression(exp_right) == true)
	    return false;
	  else if (truth_value(exp_left->data) == true)
	    return bool_conclusion(false, exp_right, F);
	}
    }
  return indeterminate;
}

static tribool	xor_expression(tribool condition, Node* exp, Fact F)
{
  Node*	exp_left = exp->left;
  Node* exp_right = exp->right;
  if (condition == true)
    {
      if (exp_right->op == FACT || exp_left->data == F)
	return false;
      else
	return bool_conclusion(false, exp_right, F);
    }
  else if (condition == false)
    {
      if (exp_right->op == FACT)
	{
	  if ((exp_left->data != F && truth_value(exp_left->data) == false)
	      || (exp_right->data != F && truth_value(exp_right->data) == false))
	    return true;
	}
      else
	{
	  if (exp_left->data == F && bool_expression(exp_right) == false)
	    return true;
	  else if (truth_value(exp_left->data) == false)
	    return bool_conclusion(true, exp->right, F);
	}
    }
  return indeterminate;
}

static tribool	not_expression(tribool condition, Node* exp, Fact F)
{
  Node* exp_right = exp->right;
  if (condition == true)
    {
      if (exp_right->op == FACT)
	return false;
      else
	return bool_conclusion(false, exp_right, F);
    }
  else if (condition == false)
    {
      if (exp_right->op == FACT)
	return true;
      else
	return bool_conclusion(true, exp_right, F);
    }
  return indeterminate;
}
