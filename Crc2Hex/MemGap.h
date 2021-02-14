#pragma once

#include <vector>
using namespace std;

class MemGap
{
    typedef struct {
        unsigned long ulStart;
        unsigned long ulEnd;
    } tMemRange;

    vector<tMemRange> vMemRangeList;
    tMemRange memRange;

public:
    MemGap(void);
    void Init(unsigned long ulStart, unsigned long ulEnd);
    int  Size(void);
    unsigned long Start(int index);
    unsigned long End(int index);
    void AddGap(unsigned long ulStart, unsigned long ulEnd);
};

