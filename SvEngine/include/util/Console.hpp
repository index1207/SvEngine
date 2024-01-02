//
// Created by han93 on 2023-12-17.
//

#pragma once

#include <iostream>
#include <syncstream>

using HANDLE = void*;

namespace sv {
    enum class LogType {
        Display, // white
        Debug,   // green
        Error    // red
    };

    enum class EncodingType {
        ANSI,
        UTF8,
    };

    class Console {
        friend class Engine;
        static void Initialize();
    public:
        static void SetOutputEncoding();
        static void Log(const char* message, LogType type = LogType::Display);
    private:
        static HANDLE m_consoleHandle;
    };
}