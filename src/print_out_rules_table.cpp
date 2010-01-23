#include <iostream>

#include "functions.h"
#include "globals.h"

static void	print_out_binary_tree(Node* node, bool tree = false)
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

void	print_out_rules_table(bool tree /*= false*/)
{
  if (g_rules.empty())
    {
      std::cout << "no rules into table" << std::endl;
      return;
    }
  std::cout << "print out rules table:" << std::endl;
  for (RulesSet::iterator it = g_rules.begin(), end = g_rules.end();
       it != end; ++it)
    {
      if (tree)
	{
	  std::cout << "---" << std::endl;
	  std::cout << "condition:" << std::endl;
	}
      print_out_binary_tree((*it)->left, tree);
      if (tree)
	std::cout << "conclusion:" << std::endl;
      else
	std::cout << "-> ";
      print_out_binary_tree((*it)->right, tree);
      if (!tree)
	std::cout << std::endl;
    }
  if (tree)
    std::cout << "---" << std::endl;
}
