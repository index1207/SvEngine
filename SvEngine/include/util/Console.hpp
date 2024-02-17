//
// Created by han93 on 2023-12-17.
//

#pragma once

#include <iostream>
#include <syncstream>

using HANDLE = void*;

struct LogCategory
{
    LogCategory(std::string_view str) : categoryName(str) {}
    std::string categoryName;
};

enum LogType {
    Display, // white
    Debug,   // green
    Error    // red
};

enum EncodingType {
    ANSI,
    UTF8,
};

class Console {
    friend class Engine;
    static void Initialize();
public:
    static void SetOutputEncoding();
    static void Log(std::string_view message, LogType type = Display);
    static void LogDisplay(std::string_view message);
    static void LogDebug(std::string_view message);
    static void LogCategory(std::string_view message);
private:
    static HANDLE m_consoleHandle;
};

MAKE_LOG_CATEGORY(Netcpp);
MAKE_LOG_CATEGORY(MYSQL);