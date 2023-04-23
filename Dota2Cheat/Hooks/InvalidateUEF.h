#pragma once
#include <MinHook.h>

namespace Hooks
{
    class InvalidateUEF
    {
    public:
        static void Create()
        {
            if (!_target_func)
            {
                _target_func = (target_func_type)GetProcAddress(GetModuleHandleW(_module), _pattern);
            }
            if (!_target_func)
                return;

            MH_STATUS status = MH_CreateHook(_target_func, &target_func_hooked, reinterpret_cast<void**>(&_original_func));
            if (status != MH_OK)
                return;

            status = MH_EnableHook(_target_func);
            if (status != MH_OK)
                return;

            _original_func(exception_filter);
        }

        static void Remove()
        {
            if (_target_func)
            {
                MH_DisableHook(_target_func);
                MH_RemoveHook(_target_func);
                _target_func = nullptr;
            }
        }

    private:
        static LPVOID WINAPI target_func_hooked(LPVOID a1)
        {
            return nullptr;
        }

        static LONG WINAPI exception_filter(LPEXCEPTION_POINTERS lpExceptionInfo)
        {
            return EXCEPTION_CONTINUE_SEARCH;
        }

        typedef decltype(&SetUnhandledExceptionFilter) target_func_type;
        static target_func_type _target_func;
        static target_func_type _original_func;
        static const wchar_t* _module;
        static const char* _pattern;
        static int _offset;
        static bool _first_time;
    };

    InvalidateUEF::target_func_type InvalidateUEF::_target_func = nullptr;
    InvalidateUEF::target_func_type InvalidateUEF::_original_func = nullptr;
    const wchar_t* InvalidateUEF::_module = L"kernel32.dll";
    const char* InvalidateUEF::_pattern = "SetUnhandledExceptionFilter";
    int InvalidateUEF::_offset = 0;
    bool InvalidateUEF::_first_time = true;
}
