#include <list>
#include <math.h>
#ifndef __VIRTUALMEMORYPAGETABLE__
#define __VIRTUALMEMORYPAGETABLE__

using namespace std;
//이거 안씀. 이걸 그냥 Job 파일에 옮겨놓음
struct pageTableEntry
{
    int pageID;
    int validBit;
    int allocationID;
    int referenceBit;
    list<int> reference_byte;
};

class VirtualMemoryPageTable
{
    int pageNum;
    int pageSize;
    pageTableEntry *table;
    VirtualMemoryPageTable(int pagenum, int pagesize)
    {
        pageNum = pagenum;
        pageSize = pagesize;
    }
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