//
// Created by han93 on 2023-12-17.
//

#pragma once

#include <iostream>
#include <syncstream>

MAKE_LOG_CATEGORY(Server);
MAKE_LOG_CATEGORY(Netcpp);
MAKE_LOG_CATEGORY(Database);

using HANDLE = void*;

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
    static void Log(std::string message, std::string category = LogServer, LogType type = Display);
private:
    static void LogDisplay(std::string_view message);
    static void LogDebug(std::string_view message);
    static void LogError(std::string_view message);
private:
    static HANDLE s_handle;
};