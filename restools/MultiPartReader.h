#pragma once
#include "Utils.h"

#define MULTIPARTREADER_PARTS_MAXIMUM 32
class MultiPartReader
{
private:
    uint8_t*    PosCurrent = nullptr;
    uint8_t*    PosStart = nullptr;
    uint8_t*    Buffer = nullptr;
    size_t      BufferSize = 0;
    uint8_t*    Parts[MULTIPARTREADER_PARTS_MAXIMUM] = {};

public:
#ifdef USING_READER
    //  These methods should only be there if this class is used as a buffer reader.
    inline MultiPartReader()
    {
    }

    MultiPartReader& operator++()
    {
        PosCurrent++;

        return *this;
    }

    MultiPartReader& operator--()
    {
        if (PosCurrent - 1 < PosStart)
            throw new std::exception("Buffer position cannot be negative!");

        PosCurrent--;

        return *this;
    }

    MultiPartReader& operator+=(const uint32_t val)
    {
        PosCurrent += val;

        return *this;
    }

    MultiPartReader& operator-=(const uint32_t val)
    {
        if (PosCurrent - val < PosStart)
            throw new std::exception("Buffer position cannot be negative!");

        PosCurrent -= val;

        return *this;
    }

    MultiPartReader& operator[](const size_t index)
    {
        if (index > BufferSize)
            throw new std::exception("Buffer index is out of range!");

        PosCurrent = PosStart + index;

        return *this;
    }

    template <typename T>
    void Write(T& val)
    {
        *((T*)PosCurrent) = val;
        PosCurrent += sizeof(T);
    }

    template <typename T>
    void Write(const T& val)
    {
        *((T*)PosCurrent) = val;
        PosCurrent += sizeof(T);
    }

    template <typename T>
    void WriteNoAdvance(T& val)
    {
        *((T*)PosCurrent) = val;
    }

    template <typename T>
    void WriteNoAdvance(const T& val)
    {
        *((T*)PosCurrent) = val;
    }

    template <typename T>
    void Write(T& val, const uint32_t partIndex)
    {
        if (partIndex >= sizeof(Parts))
            throw new std::exception("partIndex is out of bounds!");

        *((T*)Parts[partIndex]) = val;
        Parts[partIndex] += sizeof(T);
    }

    template <typename T>
    void WriteNoAdvance(T& val, const uint32_t partIndex)
    {
        if (partIndex >= sizeof(Parts))
            throw new std::exception("partIndex is out of bounds!");

        *((T*)Parts[partIndex]) = val;
    }
#endif

    MultiPartReader(FILE* filePtr, const size_t bufferSize, const bool rememberStart = false)
    {
        if (!filePtr || !bufferSize)
            return;

        Buffer = new uint8_t[bufferSize];
        const auto fileOffsetCurrent = rememberStart ? ftell(filePtr) : -1;
        const size_t bytesRead = fread(Buffer, sizeof(uint8_t), bufferSize, filePtr);
        BufferSize = bufferSize;

        if (bytesRead != bufferSize)
            throw new std::exception("Bytes read from file buffer differs from the requested size!");

        if (fileOffsetCurrent > 0)
            fseek(filePtr, fileOffsetCurrent, SEEK_SET);

        PosCurrent = Buffer;
        PosStart = PosCurrent;

        for (auto i = 0; i < (sizeof(Parts) / sizeof(Parts[0])); ++i)
            Parts[i] = PosCurrent;
    }

    inline ~MultiPartReader()
    {
        delete[] Buffer;
    }

    template <typename T>
    inline void Read(T& dest)
    {
        dest = *((T*)PosCurrent);
        PosCurrent += sizeof(T);
    }

    template <typename T>
    inline void Read(T& dest, const uint32_t partIndex)
    {
        if (partIndex >= sizeof(Parts))
            throw new std::exception("partIndex is out of bounds!");

        dest = *((T*)Parts[partIndex]);
        Parts[partIndex] += sizeof(T);
    }

    template <typename T>
    inline void ReadNoAdvance(T& dest)
    {
        dest = *((T*)PosCurrent);
    }

    template <typename T>
    inline void ReadNoAdvance(T& dest, const uint32_t partIndex)
    {
        if (partIndex >= sizeof(Parts))
            throw new std::exception("partIndex is out of bounds!");

        dest = *((T*)Parts[partIndex]);
    }

    inline void Skip(const int32_t size)
    {
        PosCurrent += size;
    }

    inline void Skip(const int32_t size, const uint32_t partIndex)
    {
        Parts[partIndex] += size;
    }
};
#pragma once
