#include "node.h"
#include "globals.h"

Node::Node(OperatorEnum _op /*= FACT*/,
	   Node* _left /*= NULL*/,
	   Node* _right /*= NULL*/,
	   Data _data /*= "NONE"*/)
  : op(_op), left(_left), right(_right), data(_data)
{
  if (data == "NONE")
    data = g_operators[op].desc;
}

Node::~Node()
{
  if (left != NULL)
    delete left;
  if (right != NULL)
    delete right;
}
