#include "PhysicalMemoryTree.h"

void PhysicalMemoryTree::make_child(TreeNode *&current)
{
    TreeNode *leftNode = new TreeNode(current->frameNum / 2, current);
    TreeNode *rightNode = new TreeNode(current->frameNum / 2, current);
    leftNode->buddy = rightNode;
    rightNode->buddy = leftNode;

    current->left = leftNode;
    current->right = rightNode;
}

void PhysicalMemoryTree::sumup_child(TreeNode *&current)
{
    delete current->left;
    current->left = nullptr;
    delete current->right;
    current->right = nullptr;
}

pair<TreeNode *, TreeNode *> PhysicalMemoryTree::childPairReturn(TreeNode *&current)
{
    return make_pair(current->left, current->right);
}