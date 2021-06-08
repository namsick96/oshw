#include <list>
#include <math.h>
#ifndef __PHYSICALMEMORYTREE__
#define __PHYSICALMEMORYTREE__

using namespace std;

class TreeNode
{
public:
    int frameNum;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;
    TreeNode *parent;
    TreeNode *buddy = nullptr;

    int allocationID;

    TreeNode(int framenum, TreeNode *nodeParent)
    {
        frameNum = framenum;
        parent = nodeParent;
    }
};

class PhysicalMemoryTree
{
public:
    // int reference_bit;
    // list<int> reference_byte;

    // int frameNum;
    int allocationID;
    TreeNode *root;
    int wholeFramesize;
    PhysicalMemoryTree(int framenum)
    {
        wholeFramesize = framenum;
        allocationID = 0;
        root = new TreeNode(framenum, nullptr);
    }
    void make_child(TreeNode *&current);
    void sumup_child(TreeNode *&current);
    pair<TreeNode *, TreeNode *> childPairReturn(TreeNode *&current);
};
/*
class PageBundle
{
public:
    int accessTime;
    int allocateTime;
    int accessCount;
    int reference_bit;
    list<int> reference_byte;

    int buddyPageNum;
    int pageNum;
    int aid;
    int index;

    int pbId;
    int parentId;

    PageBundle(int page, int allocationId, int ind)
    {
        pageNum = page;
        aid = allocationId;
        index = ind;
    }

    PageBundle()
    {
    }

    void calcBuddySize(int physicalMemSize);

    double calcReferenceValue();
};
*/

#endif //PRACTICE1_PAGEBUNDLE_H