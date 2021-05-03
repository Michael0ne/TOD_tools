#pragma once
#include "BaseType.h"

class ListType : public BaseType
{
protected:
    BaseType       *m_ListElementsType;
    unsigned int    m_ListElementSize;
    bool            m_ComplexType;

public:
    ListType(BaseType* elementsType);	//	@870630
    virtual        ~ListType();

    virtual int     stub2(int*, int*);	//	@870CD0
    virtual void*   stub3(void*) const;	//	@4C8A50
    virtual void    stub4(char*);	//	@8718D0
    virtual void    stub5(int*, int*);	//	@8717D0
    virtual String& stub6(String&, void*, int) const;	//	@8707E0
    virtual int     stub7(char*, void*) const;	//	@871710
    virtual int     stub9(char*, char*);	//	@870C60
    virtual int     stub10(char*, char*);	//	@8711A0
    virtual int     stub11(char*, String&, int);	//	@870AE0
    virtual int     stub12(char*, char*, int*);	//	@870560
    virtual bool    stub16(void*, void*) const;	//	@870D50
    virtual void    stub17(const char* const operation, int* outopid, BaseType** outoprestype, char* a4) const;	//	@870DE0
    virtual void    stub18(int operationId, void* params) const;	//	@8712F0
    virtual char    stub19(int, int);	//	@870730
};