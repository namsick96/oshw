#include <list>
#ifndef __PAGEMETA__
#define __PAGEMETA__

using namespace std;

class PageMeta
{
public:
    int accessTime;
    int allocateTime;
    int accessCount;

    list<int> reference_byte;

    int buddyPageNum;
    int pageNum;
    int allocationID;

    int validBit;
    int referenceBit;

    int pbId;
    int parentId;

    PageMeta(int page, int allocationid)
    {
        pageNum = page;
        allocationID = allocationid;
        validBit = 0;
        referenceBit = 0;
    }

    PageMeta()
    {
    }

    void calcBuddySize(int physicalMemSize);

    double calcReferenceValue();
};

#endif