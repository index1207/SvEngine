//
// Created by han93 on 2023-12-17.
//

#pragma once

#include <iostream>
#include <syncstream>

MAKE_LOG_CATEGORY(Netcpp);
MAKE_LOG_CATEGORY(Database);
MAKE_LOG_CATEGORY(Temp);
MAKE_LOG_CATEGORY(IO);

using HANDLE = void*;

enum LogType {
    Log, // white
    Warning,
    Debug,   // green
    Error    // red
};

enum EncodingType {
    ANSI,
    UTF8,
};

class Console {
    friend class Engine;

    using ColorOperation = std::ostream&(std::ostream&);

    static void Initialize();
public:
    static void SetOutputEncoding();
    static void Log(String category, LogType type, String message);
private:
    static void Print(ColorOperation color, String message, bool ln = true);
    static void LogDisplay(String category, String message);
    static void LogWarning(String category, String message);
    static void LogDebug(String category, String message);
    static void LogError(String category, String message);
private:
    static HANDLE s_handle;
};