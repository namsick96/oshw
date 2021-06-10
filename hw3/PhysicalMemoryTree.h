#include <list>
#include <math.h>
#include <stack>
#include "Job.h"
#ifndef __PHYSICALMEMORYTREE__
#define __PHYSICALMEMORYTREE__

using namespace std;

class TreeNode
{
    friend class PhysicalMemoryTree;

public:
    int frameNum;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;
    TreeNode *parent;
    TreeNode *buddy = nullptr;

    bool splited = false;

    int allocationID = -1;
    int pageID = -1;

    TreeNode(int framenum, TreeNode *nodeParent)
    {
        frameNum = framenum;
        parent = nodeParent;
    }
    void make_child();
    void sumup_child();
};

class PhysicalMemoryTree
{
    friend class TreeNode;

public:
    // int reference_bit;
    // list<int> reference_byte;

    // int frameNum;
    int physical_final_allocationID;
    TreeNode *root;
    int wholeFramenum;
    list<int> allocID_stack;
    list<pair<Job *, int> > allocated_list;
    PhysicalMemoryTree(int framenum)
    {
        wholeFramenum = framenum;
        physical_final_allocationID = 0;
        root = new TreeNode(framenum, nullptr);
    }
    //void make_child(TreeNode *&current);
    //void sumup_child(TreeNode *&current);
    void wrapUp(TreeNode *&current);
    pair<TreeNode *, TreeNode *> childPairReturn(TreeNode *&current);
    TreeNode *findAllocID(TreeNode *&current, int alloc_id);
    TreeNode *findProperNode(TreeNode *&current, int frame_num);
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