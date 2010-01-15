#ifndef __NODE_H__
# define __NODE_H__

# include "typedefs.h"

struct	Node
{
  OperatorEnum	op;
  Node*		left;
  Node*		right;
  Data		data;

  Node(OperatorEnum _op = FACT,
       Node* _left = NULL,
       Node* _right = NULL,
       Data _data = "NONE");
  ~Node();
};

#endif // !__NODE_H__
