#pragma once
#include "DataType.h"

class QuaternionType : public DataType
{
public:
 QuaternionType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);
 virtual ~QuaternionType();

 virtual void* stub3(void*) const override; // @865890
 virtual String& stub6(String&, void*, int) const override; // @867810
 virtual int  stub7(char*, void*) const override; // @8658F0
 virtual void stub13(int, void*, int, int, int, void* const) const override; // @8659E0
 virtual void stub14(int*, int, void*, int, int, int) const override; // @865A40
 virtual bool stub16(void*, void*) const override; // @8678E0
 virtual void stub17(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const override; // @865C10
 virtual void stub18(int operationId, void* params) const override; // @866090
 virtual bool stub20(void*) const override; // @865AC0

 void* operator new (size_t size)
 {
  return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
 }
 void operator delete (void* ptr)
 {
  if (ptr)
   MemoryManager::ReleaseMemory(ptr, 0);
  ptr = nullptr;
 }
};