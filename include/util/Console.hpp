//
// Created by han93 on 2023-12-17.
//

#pragma once

#include <string>

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
        static void Log(std::string message, LogType type = LogType::Display);
    };
}