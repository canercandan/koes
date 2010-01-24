#include <stdexcept>
#include "functions.h"
#include "node.h"
#include "typedefs.h"
#include <iostream>
typedef Boolean (*op_exp)(Boolean, Node*, Fact);

static Boolean	and_expression(Boolean, Node*, Fact);
static Boolean	or_expression(Boolean, Node*, Fact);
static Boolean	xor_expression(Boolean, Node*, Fact);
static Boolean	not_expression(Boolean, Node*, Fact);

static op_exp	expressionArray[] = {
  and_expression,
  or_expression,
  xor_expression,
  not_expression
};

Boolean	bool_conclusion(Boolean condition, Node* exp, Fact F)
{
  if (!(exp->op >= AND && exp->op <= NOT))
    throw std::runtime_error("bad operator");
  return (expressionArray[exp->op - 2])(condition, exp, F);
}

static Boolean	and_expression(Boolean condition, Node* exp, Fact F)
{
  Node*	exp_left = exp->left;
  Node* exp_right = exp->right; 

  if (condition == TRUE)
    {
      if (exp_right->op == FACT || exp_left->data == F)
	return TRUE;
      else
	return bool_conclusion(TRUE, exp->right, F);
    }
  else if (condition == FALSE)
    {
      if (exp_right->op == FACT)
	{
	  if (exp_left->data == F && truth_value(exp_right->data) == TRUE)
	    return FALSE;
	}
      else
	{
	  if (exp_left->data == F && bool_expression(exp_right) == TRUE)
	    return FALSE;
	  else if (truth_value(exp_left->data) == TRUE)
	    return bool_conclusion(FALSE, exp->right, F);
	}
    }
  return UNKNOWN;
}

static Boolean	or_expression(Boolean condition, Node* exp, Fact F)
{
  Node*	exp_left = exp->left;
  Node* exp_right = exp->right; 
  if (condition == TRUE)
    {
      if (exp_right->op == FACT)
	{
	  if ((exp_left->data != F && truth_value(exp_left->data) == FALSE) 
	      || (exp_right->data != F && truth_value(exp_right->data) == FALSE))
	    return TRUE;
	}
      else
	{
	  if (exp_left->data == F && bool_expression(exp_right) == FALSE)
	    return TRUE;
	  else if (truth_value(exp_left->data) == FALSE) 
	    return bool_conclusion(TRUE, exp_right, F);
	}
    }
  else if (condition == FALSE)
    {
      if (exp_right->op == FACT)
	{
	  if ((exp_left->data != F && truth_value(exp_left->data) == TRUE) 
	      || (exp_right->data != F && truth_value(exp_right->data) == TRUE))
 	    return FALSE;
	}
      else
	{
	  if (exp_left->data == F && bool_expression(exp_right) == TRUE)
	    return FALSE;
	  else if (truth_value(exp_left->data) == TRUE) 
	    return bool_conclusion(FALSE, exp_right, F);
	}
    }
  return UNKNOWN;
}

static Boolean	xor_expression(Boolean condition, Node* exp, Fact F)
{
  Node*	exp_left = exp->left;
  Node* exp_right = exp->right; 
  if (condition == TRUE)
    {
      if (exp_right->op == FACT || exp_left->data == F)
	return FALSE;
      else
	return bool_conclusion(FALSE, exp_right, F);
    }
  else if (condition == FALSE)
    {
      if (exp_right->op == FACT)
	{
	  if ((exp_left->data != F && truth_value(exp_left->data) == FALSE) 
	      || (exp_right->data != F && truth_value(exp_right->data) == FALSE))
	    return TRUE;
	}
      else
	{
	  if (exp_left->data == F && bool_expression(exp_right) == FALSE)
	    return TRUE;
	  else if (truth_value(exp_left->data) == FALSE)
	    return bool_conclusion(TRUE, exp->right, F);
	}     
    }
  return UNKNOWN;
}

static Boolean	not_expression(Boolean condition, Node* exp, Fact F)
{
  Node* exp_right = exp->right; 
  if (condition == TRUE)
    {
      if (exp_right->op == FACT)
	return FALSE;
      else
	return bool_conclusion(FALSE, exp_right, F);
    }
  else if (condition == FALSE)
    {
      if (exp_right->op == FACT)
	return TRUE;
      else
	return bool_conclusion(TRUE, exp_right, F);
    }
  return UNKNOWN;
}
