#include "PhysicalMemoryTree.h"

void TreeNode::make_child()
{
    TreeNode *leftNode = new TreeNode(this->frameNum / 2, this);
    TreeNode *rightNode = new TreeNode(this->frameNum / 2, this);
    leftNode->buddy = rightNode;
    rightNode->buddy = leftNode;

    this->left = leftNode;
    this->right = rightNode;
    this->splited = true;
}

void TreeNode::sumup_child()
{
    delete this->left;
    this->left = nullptr;
    delete this->right;
    this->right = nullptr;
    this->splited = false;
}

pair<TreeNode *, TreeNode *> PhysicalMemoryTree::childPairReturn(TreeNode *&current)
{
    return make_pair(current->left, current->right);
}
/*
TreeNode *PhysicalMemoryTree::findLeft(TreeNode *&current)
{
    return current->left;
}

TreeNode *PhysicalMemoryTree::findRight(TreeNode *&current)
{
    return current->right;
}*/

TreeNode *PhysicalMemoryTree::findAllocID(TreeNode *&current, int alloc_id)
{
    if (current == nullptr)
    {
        return current;
    }
    else if (current->allocationID == alloc_id)
    {
        return current;
    }
    else
    {
        TreeNode *leftRecur = findAllocID(current->left, alloc_id);
        TreeNode *rightRecur = findAllocID(current->right, alloc_id);
        if (leftRecur != nullptr)
        {
            return leftRecur;
        }
        else
        {
            return rightRecur;
        }
    }
}
TreeNode *PhysicalMemoryTree::findProperNode(TreeNode *&current, int frame_num)
{
    //마지막 level
    if (current == nullptr)
    {
        return current;
    }
    //이미 allocated되어 있을 때 또는 제일 아래로 내려가서 frame개수가 1 미만일떄
    if (current->allocationID != -1 || current->frameNum < 1)
    {
        return nullptr;
    }
    //이미 쪼개져 있을 때
    if (current->allocationID == -1 && current->frameNum / 2 >= frame_num && current->splited)
    {
        TreeNode *leftRecur = findProperNode(current->left, frame_num);
        if (leftRecur != nullptr)
        {
            return leftRecur;
        }
        TreeNode *rightRecur = findProperNode(current->right, frame_num);
        if (rightRecur != nullptr)
        {
            return rightRecur;
        }
        /*
        if (leftRecur != nullptr)
        {
            return leftRecur;
        }
        else
        {
            return rightRecur;
        }*/
    }

    //노드를 쪼개야할때
    if (current->allocationID == -1 && current->frameNum / 2 >= frame_num && !current->splited)
    {
        current->make_child();
        TreeNode *leftRecur = findProperNode(current->left, frame_num);
        if (leftRecur != nullptr)
        {
            //current->right = nullptr;
            return leftRecur;
        }
        TreeNode *rightRecur = findProperNode(current->right, frame_num);
        if (rightRecur != nullptr)
        {
            return rightRecur;
        }
        /*if (leftRecur != nullptr)
        {
            //current->right = nullptr;
            return leftRecur;
        }
        else
        {
            //current->left = nullptr;
            return rightRecur;
        }*/
    }
    if (current->allocationID == -1 && current->frameNum >= frame_num && current->frameNum / 2 < frame_num && !current->splited)
    {
        //node 안쪼개도 됨
        return current;
    }

    return nullptr;
}

void PhysicalMemoryTree::wrapUp(TreeNode *&current)
{
    if (current->parent == nullptr)
    {
        return;
    }
    if (current->buddy->allocationID == -1 && !current->buddy->splited)
    {
        current = current->parent;
        delete current->left;
        current->left = nullptr;
        delete current->right;
        current->right = nullptr;
        current->splited = false;
        wrapUp(current);
        return;
    }
    else
    {
        current->allocationID = -1;
        current->pageID = -1;
        return;
    }
}