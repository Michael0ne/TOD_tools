#pragma once
#include "MemoryManager.h"

namespace Input
{
#define INPUT_KEYBOARD_BUFFERS_COUNT 30

    class Keyboard
    {
    public:
        unsigned char   m_ButtonStates[256];
        unsigned char   m_ButtonStates_1[256];
        bool            m_Acquired;
        IDirectInputDevice8* m_DirectInputDeviceInterface;
        IDirectInput8* m_DirectInputDevice;
        DIDEVICEOBJECTDATA* m_DataBuffer;
        unsigned int   m_DataBufferSize;

    public:
        Keyboard();  // @43AD80
        ~Keyboard(); // @43AF20

        void* operator new(size_t size)
        {
            return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
        }
        void operator delete(void* ptr)
        {
            if (ptr)
                MemoryManager::ReleaseMemory(ptr, 0);
            ptr = nullptr;
        }

        void     Process(); // @43AF60
        void     Reset(); // @43A740
        void     ResetKeyboardButtons(); // @43A750
        void     UnacquireAndResetKeyboard(); // @43A790

        static const char* KeyboardButtonToStr(int keyCode); // @43A7D0

private:
        static const char*  KeyboardButtons[];
    };
}

extern Input::Keyboard* g_InputKeyboard;

ASSERT_CLASS_SIZE(Input::Keyboard, 532);