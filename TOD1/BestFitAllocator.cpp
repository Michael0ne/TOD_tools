#include "BestFitAllocator.h"
#include "LogDump.h"

unsigned int BestFitAllocator::MinimumSize;

uint32_t BestFitAllocator::FindSuitableBlock(const uint32_t size) const
{
    uint32_t j = 0;
    uint32_t i = 19;

    do
    {
        const uint32_t blockIndex = j + (i - j) / 2;
        if (size <= field_24[blockIndex].field_4)
            i = j + (i - j) / 2;

        if (size > field_24[blockIndex].field_4)
            j = blockIndex;
    } while (i > j + 1);

    if (size > field_24[j].field_4)
        return i;
    else
        return j;
}

uint32_t* BestFitAllocator::_479290(uint32_t* ptr, const uint32_t a2, const uint32_t requestedSize)
{
    const uint32_t requestedSizeAligned = ALIGN_16BYTESUP(requestedSize) - 8;
    if (a2 <= requestedSizeAligned - 24)
        return nullptr;

    if ((*(ptr + 1) & 0x40000000) == 0)
        _478E80(ptr);

    uint32_t* dataPtr = ptr + ALIGN_16BYTESUP(requestedSize);
    uint32_t* nextDataPtr = (uint32_t*)*ptr;

    dataPtr[0] = *ptr;

    *(nextDataPtr + 4) ^= ( *(nextDataPtr + 4) ^ ((uint32_t)dataPtr >> 2) ) ^ 0x3FFFFFFF;

    dataPtr[1] = (uint32_t)(ptr + ALIGN_16BYTESUP(requestedSize) + 12) & 0x3FFFFFFF | 0x80000000;
    dataPtr[1] = (dataPtr[1] ^ ((uint32_t)ptr >> 2)) & 0x3FFFFFFF;
    dataPtr[4] = 0;
    dataPtr[5] = 0;

    const bool _b0 = (*(ptr + 1) & 0x40000000) == 0;

    *ptr = (uint32_t)dataPtr;

    if (_b0)
        _478F70(ptr);

    _478F70(dataPtr);

    BlocksFree++;

    return ptr + ALIGN_16BYTESUP(requestedSize);
}

uint32_t* BestFitAllocator::_478E80(uint32_t* ptr)
{
    uint32_t* nextDataPtr = (uint32_t*)*ptr;
    if (*ptr <= (uint32_t)ptr)
        nextDataPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

    const uint32_t blockId = FindSuitableBlock(nextDataPtr - ptr - 8);
    uint32_t* dataPtr = (uint32_t*)*(ptr + 5);
    if (dataPtr)
    {
        dataPtr[4] = *(ptr + 4);
        uint32_t* _ptr = (uint32_t*)*(ptr + 4);
        if (_ptr)
            _ptr[5] = *(ptr + 5);

        return _478910(blockId);
    }

    dataPtr = (uint32_t*)*(ptr + 4);
    if (!dataPtr)
    {
        field_24[blockId].field_8--;

        uint32_t* _ptr = (uint32_t*)*(ptr + 2);
        if (_ptr)
            _ptr[3] = *(ptr + 3);

        _ptr = (uint32_t*)*(ptr + 3);
        if (_ptr)
            _ptr[2] = *(ptr + 2);

        uint32_t* blockPtr = field_24[blockId].DataPtr;
        if (ptr == blockPtr)
        {
            blockPtr = (uint32_t*)*(ptr + 2);
            return _478910(blockId);
        }

        return _478910(blockId);
    }

    dataPtr[2] = *(ptr + 2);
    dataPtr[3] = *(ptr + 3);

    uint32_t* _ptr = (uint32_t*)*(ptr + 2);
    if (_ptr)
        _ptr[3] = (uint32_t)dataPtr;

    _ptr = (uint32_t*)*(ptr + 3);
    if (_ptr)
        _ptr[2] = (uint32_t)dataPtr;

    uint32_t* blockPtr = field_24[blockId].DataPtr;
    if (ptr == blockPtr)
        blockPtr = dataPtr;

    dataPtr[5] = 0;

    return _478910(blockId);
}

BestFitAllocator::BestFitAllocator()
{
    MESSAGE_CLASS_CREATED(BestFitAllocator);

    uint32_t k = 32;
    for (uint32_t i = 20; i; i--)
    {
        field_24[i].DataPtr = nullptr;
        field_24[i].field_4 = k;
        field_24[i].field_8 = 0;

        k = (uint32_t)((float)k * 1.5f);
    }

    field_24[19].field_4 = 0x7FFFFFFF;  //  NOTE: NaN.

    DataPtr = nullptr;
    BytesAvailable = NULL;
    BlocksFree = NULL;
    BlocksUsed = NULL;

    MinimumSize = 16;
}

void* BestFitAllocator::Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber)
{
    return AllocateAligned(size, 8, fileName, fileLineNumber);
}

void* BestFitAllocator::AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber)
{
    stub9();

    if (alignment < 8)
        alignment = 8;

    if (size < MinimumSize)
        size = MinimumSize;

    const uint32_t sizeAligned = ALIGN_4BYTESUP(size);
    uint32_t suitableBlockIndex = FindSuitableBlock(sizeAligned);

    if (suitableBlockIndex >= 20)
    {
        stub9();
        return nullptr;
    }

    BlockInfo* block = &field_24[suitableBlockIndex];
    uint32_t* dataPtr;
    void* newDataPtr;

    while (true)
    {
        dataPtr = block->DataPtr;
        if (block->DataPtr)
            break;

        suitableBlockIndex++;
        block++;

        if (suitableBlockIndex >= 20)
        {
            stub9();
            return nullptr;
        }

        newDataPtr = (void*)stub25(dataPtr, sizeAligned, fileName, fileLineNumber, alignment);
        if (newDataPtr)
            break;

        dataPtr = (uint32_t*)dataPtr[2];
        if (!dataPtr)
        {
            suitableBlockIndex++;
            block++;

            if (suitableBlockIndex >= 20)
            {
                stub9();
                return nullptr;
            }
        }
    }

    stub9();
    return newDataPtr;
}

void BestFitAllocator::Free(void* ptr)
{
    stub9();

    if (!ptr)
        return;

    uint32_t* dataPtr = (uint32_t*)ptr - 2;
    *((uint32_t*)ptr - 1) &= ~0x40000000;

    _478F70((uint8_t*)ptr - 8);

    uint32_t* dataPtrCopy = (uint32_t*)*dataPtr;
    if (*dataPtr <= (uint32_t)dataPtr)
        dataPtrCopy = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

    BlocksFree++;
    BytesAvailable += dataPtr - dataPtrCopy;
    BlocksUsed--;

    _4790C0(dataPtr, 0);
    _479180(dataPtr);

    stub9();

}

void BestFitAllocator::FreeAligned(void* ptr)
{
    Free(ptr);
}

#pragma message(TODO_IMPLEMENTATION)
void* BestFitAllocator::Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber)
{
    uint32_t sizeAligned = newsize;
    if (newsize < MinimumSize)
        if (newsize)
            sizeAligned = MinimumSize;

    sizeAligned = ALIGN_4BYTESUP(sizeAligned);

    if (!oldptr)
        return Allocate_A(sizeAligned, fileName, fileLineNumber);

    if (!sizeAligned)
    {
        Free(oldptr);

        return nullptr;
    }

    uint32_t* nextDataPtr = (uint32_t*)((uint32_t*)oldptr - 2);
    const uint32_t* nextDataPtrCopy = (uint32_t*)((uint32_t*)oldptr - 2);
    if (nextDataPtr <= (uint32_t*)((uint32_t*)oldptr - 2))
        nextDataPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

    const uint32_t dataSize = (uint8_t*)nextDataPtr - (uint8_t*)oldptr;
    if (sizeAligned > dataSize)
    {
        _4790C0(nextDataPtrCopy);   //  NOTE: this WILL modify the pointer data!

        nextDataPtr = (uint32_t*)*nextDataPtr;
        if (*nextDataPtrCopy <= (uint32_t)nextDataPtrCopy)
            nextDataPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

    }
}

uint32_t BestFitAllocator::stub8(uint32_t* ptr)
{
    if (!ptr)
        return (uint32_t)ptr;

    uint32_t* nextDataPtr = (uint32_t*)(ptr - 2);
    if (nextDataPtr <= (uint32_t*)(ptr - 2))
        nextDataPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

    return nextDataPtr - ptr;
}

void BestFitAllocator::stub9()
{
    return;
}

void BestFitAllocator::CallMethodAtOffset20()
{
    return;
}

void BestFitAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
    Allocator::SetNameAndAllocatedSpaceParams(bufferptr, name, size);

    uint32_t* dataBlockPtr = (uint32_t*)(ALIGN_16BYTESUP((uint32_t)AllocatedSpacePtr) - 8);
    dataBlockPtr[0] = (uint32_t)dataBlockPtr;
    dataBlockPtr[1] = ((uint32_t)dataBlockPtr >> 2) & 0x3FFFFFFF | 0x80000000;  //  NOTE: bits: [30] -> is being used, [31] -> ? (set to 1 here).
    //  NOTE: neat trick to store additional info in the pointer without wasting space for additional member variable:
    //      since we know that all pointers are aligned by 16 bytes (lowest 2 bits) we can drop them and store that
    //      additional info in the highest 2 bits.
    dataBlockPtr[2] = 0;
    dataBlockPtr[3] = 0;
    dataBlockPtr[4] = 0;
    dataBlockPtr[5] = 0;

    const uint32_t bestBlock = FindSuitableBlock((uint32_t)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize - (uint8_t*)dataBlockPtr - 8));

    field_24[bestBlock].DataPtr = dataBlockPtr;
    field_24[bestBlock].field_8 = 1;

    BytesAvailable = 0;
    BlocksUsed = 0;
    DataPtr = dataBlockPtr;
    BlocksFree = 1;
}

const int BestFitAllocator::GetFreeMemory() const
{
    return BytesAvailable;
}

const int BestFitAllocator::GetAllocatedElementsTotal() const
{
    return BlocksUsed;
}

const int BestFitAllocator::GetAllocationsMadeTotal() const
{
    return BlocksUsed;
}

const int BestFitAllocator::stub20() const
{
    return BlocksFree;
}

#pragma message(TODO_IMPLEMENTATION)
const int BestFitAllocator::stub21() const
{
    return NULL;
}

const int BestFitAllocator::GetAvailableMemory() const
{
    int32_t memoryAvailable = 0;

    for (uint32_t i = 20; i; i--)
    {
        for (uint32_t* dataPtr = field_24[i].DataPtr; dataPtr; dataPtr = (uint32_t*)dataPtr[2])
        {
            uint32_t* nextDataPtr = (uint32_t*)*dataPtr;
            if (*dataPtr <= (uint32_t)dataPtr)
                nextDataPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

            const uint32_t dataBlockSize = nextDataPtr - dataPtr - 8;
            uint32_t* nextBlockPtr = (uint32_t*)dataPtr[4];

            while (nextBlockPtr)
            {
                memoryAvailable += dataBlockSize;
                nextBlockPtr = (uint32_t*)nextBlockPtr[4];
            }
        }
    }

    return memoryAvailable;
}

void BestFitAllocator::Dump() const
{
    LogDump::LogA("BestFitAllocator contents:\n");
    LogDump::LogA("-----------------------------\n");

    uint32_t* dataPtr = DataPtr;
    for (uint32_t i = 0; i < BlocksFree + BlocksUsed; ++i)
    {
        uint32_t* nextDataPtr = (uint32_t*)*dataPtr;
        if (*dataPtr <= (uint32_t)dataPtr)
            nextDataPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

        const uint32_t dataSize = nextDataPtr - dataPtr - 8;
        const bool isUsed = dataPtr[1] & 0x40000000;

        LogDump::LogA("%d\t: %x\t : %s\t : %d\n",
            i,
            dataPtr,
            isUsed ? "used" : "free",
            dataSize
        );

        dataPtr = (uint32_t*)*dataPtr;
    }

    LogDump::LogA("-----------------------------\n");
}

int BestFitAllocator::stub24(uint32_t* ptr, const uint32_t size, const char* const fileName, const unsigned int fileLineNumber)
{
    return stub25(ptr, size, fileName, fileLineNumber, 8);
}

#pragma message(TODO_IMPLEMENTATION)
int BestFitAllocator::stub25(uint32_t* ptr, const uint32_t size, const char* const fileName, const unsigned int fileLineNumber, int alignment)
{
    return NULL;
}

int* BestFitAllocator::GetMemoryContents() const
{
    return (int32_t*)DataPtr;
}

const int BestFitAllocator::stub27() const
{
    return DataPtr[1] * 4;
}

int BestFitAllocator::stub28(int* a1) const
{
    return (uint32_t*)*a1 == DataPtr ? NULL : *a1;
}

const int BestFitAllocator::stub29(int* a1) const
{
    return DataPtr ? NULL : (4 * a1[1]);
}

char BestFitAllocator::OwnsPointer(int* a1) const
{
    return (a1[1] >> 30) & 1;
}

char BestFitAllocator::stub31(uint32_t* ptr, const uint32_t size, const uint32_t alignment) const
{
    uint32_t* nextDataPtr = (uint32_t*)*ptr;
    if (*ptr <= (uint32_t)ptr)
        nextDataPtr = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);

    const uint32_t dataBlockSize = nextDataPtr - ptr - 8;
    const uint32_t dataPtrModulo = (uint32_t)(ptr + 8) % alignment;
    uint32_t closestAlignedSize = alignment - dataPtrModulo + 24;

    uint32_t possibleSize = alignment - dataPtrModulo - 8;
    while (possibleSize < MinimumSize)
    {
        possibleSize += alignment;
        closestAlignedSize += alignment;
    }

    if (dataPtrModulo)
        return dataBlockSize >= (closestAlignedSize + size);
    else
        return dataBlockSize >= size;
}

int BestFitAllocator::stub32(int a1) const
{
    return a1 + 8;
}

int BestFitAllocator::stub33(int a1) const
{
    return a1 - 8;
}

#pragma message(TODO_IMPLEMENTATION)
char BestFitAllocator::TryExpandBy(uint8_t* ptr, uint32_t size)
{
    if (!size)
        return true;

    if (size < MinimumSize)
        size = MinimumSize;

    const uint32_t sizeAligned = ALIGN_4BYTESUP(size);
    if (!ptr)
    {
        void* newPtr = Allocate_A(sizeAligned, __FILE__, __LINE__);
        if (!newPtr)
            return false;

        Free(newPtr);
        return true;
    }

    const uint32_t* nextDataPtr = (uint32_t*)(ptr - 8);
    uint32_t* nextData = (uint32_t*)*(ptr - 8);

    while (true)
    {
        if (nextData <= nextDataPtr)
            nextData = (uint32_t*)((uint8_t*)AllocatedSpacePtr + AllocatedSpaceSize);
        const uint32_t dataSize = nextData - (uint32_t*)ptr;
        const uint32_t dataSizeCopy = dataSize;
        if (sizeAligned <= dataSize)
            break;
    }
}

int BestFitAllocator::GetMemoryReserved()
{
    return BlocksFree + BlocksUsed;
}