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
    void __fastcall Init(unsigned long ulStart, unsigned long ulEnd);
    int __fastcall Size(void);
    unsigned long __fastcall Start(int index);
    unsigned long __fastcall End(int index);
    void __fastcall AddGap(unsigned long ulStart, unsigned long ulEnd);
};

