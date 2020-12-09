#include "ResourcesTypes.h"

namespace Types
{
	namespace Resources
	{

#pragma message(TODO_IMPLEMENTATION)
		Base::Base(const char* type, void* (*creator)())
		{
			MESSAGE_CLASS_CREATED(Base);
		}

		void Base::SetResourceAlignment(unsigned int size, unsigned int index)
		{
			m_Alignment[index] = size;

			if (ResourceAlignment[index] < size)
				ResourceAlignment[index] = size;
		}

		unsigned int Base::ResourceAlignment[3] = { NULL, NULL, NULL };

	}
}