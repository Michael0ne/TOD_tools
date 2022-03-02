#pragma once
#include "DataType.h"

class ListType : public DataType
{
protected:
    DataType       *m_ListElementsType;
    unsigned int    m_ListElementSize;
    bool            m_ComplexType;

public:
    ListType(DataType* elementsType); // @870630
    virtual        ~ListType();

    virtual int     stub2(int*, int*); // @870CD0
    virtual void*   ReturnNew(void*) const; // @4C8A50
    virtual void    Delete(char*); // @8718D0
    virtual void    stub5(int*, int*); // @8717D0
    virtual String& PrintFormattedValue(String&, void*, int) const; // @8707E0
    virtual int     StrToType(char*, void*) const; // @871710
    virtual int     stub9(char*, char*); // @870C60
    virtual int     Copy(char*, char*); // @8711A0
    virtual int     stub11(char*, String&, int); // @870AE0
    virtual int     stub12(char*, char*, int*); // @870560
    virtual bool    NotEqualTo(void*, void*) const; // @870D50
    virtual void    ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const; // @870DE0
    virtual void    PerformOperation(int operationId, void* params) const; // @8712F0
    virtual char    IsReferenced(int, int); // @870730
};