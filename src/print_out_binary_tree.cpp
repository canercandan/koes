#include <iostream>

#include "functions.h"
#include "globals.h"

void	print_out_binary_tree(Node* node, bool tree /*= false*/)
{
  if (node == NULL)
    return;

  if (node->op == FACT)
    {
      if (tree)
	std::cout << "FACT [";
      std::cout << node->data;
      if (tree)
	std::cout << "]" << std::endl;
      else
	std::cout << " ";
      return;
    }

  if (tree)
    {
      std::cout << "OP [";
      std::cout << node->data;
      std::cout << "]" << std::endl;
    }

  if (node->left != NULL)
    print_out_binary_tree(node->left, tree);

  if (!tree)
    std::cout << node->data << " ";

  if (node->right != NULL)
    print_out_binary_tree(node->right, tree);
}
