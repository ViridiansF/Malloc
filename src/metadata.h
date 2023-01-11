typedef struct metaData
{
    size_t allocatedMemory;
    void* ptrData;
    bool blocIsFree; 
    struct metaData* next;
} metaData;

