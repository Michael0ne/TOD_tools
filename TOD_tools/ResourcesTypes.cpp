#include "ResourcesTypes.h"

namespace Types
{
	namespace Resources
	{
		TypeInfo::TypeInfo(const char* _typename, void* (*creator)())
		{
			MESSAGE_CLASS_CREATED(TypeInfo);
		}
	}
}