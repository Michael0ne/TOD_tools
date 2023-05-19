#pragma once
#include "Globals.h"
#include "MemoryManager.h"

namespace Input
{
#define INPUT_MOUSE_BUFFERS_COUNT 30
#define INPUT_MOUSE_MAX_BUTTONS 10

    enum eMouseButtons {
        MOUSE_BUTTON_0 = 0,
        MOUSE_BUTTON_1,
        MOUSE_BUTTON_2,
        MOUSE_BUTTON_3,
        MOUSE_BUTTON_4,
        MOUSE_BUTTON_5,
        MOUSE_BUTTON_6,
        MOUSE_BUTTON_7,
        MOUSE_BUTTON_WHEEL_UP,
        MOUSE_BUTTON_WHEEL_DOWN
    };

    //---------- Wrapper around DirectInput Mouse ----------------
    class Mouse
    {
    public:
        unsigned int   m_MouseButtons[10];
        int      m_Position_Z;
        tagPOINT    m_FullscreenMousePosition;
        tagPOINT    m_WindowedMousePosition;
        int      m_Position_X;
        int      m_Position_Y;
        int      m_WindowsCaptured;
        bool     m_ShouldBeProcessed;
        LPDIRECTINPUT8   m_DirectInputInterface;
        LPDIRECTINPUTDEVICE8 m_DirectInputDevice;
        bool     m_Acquired;
        LPDIDEVICEOBJECTDATA    m_Buffer;
        unsigned int   m_BufferSize;

    public:
        Mouse(); // @43B4E0
        ~Mouse();

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

        void     _Acquire(); // @43B370
        void     UnacquireAndReset(); // @43B390
        void     SetWindowCapture(HWND window); // @43B3C0
        void     ReleaseWindowCapture(); // @43B3F0
        void     ResetButtonsState(); // @43B410
        void     Process(); // @43B670

        static const char* MouseButtonToStr(unsigned int button); // @43B440
        static const char* MousePositionTostr(unsigned int axis); // @43B4C0
    };

    ASSERT_CLASS_SIZE(Mouse, 96);
}

extern Input::Mouse* g_InputMouse;
extern const char* g_MouseButtons[];