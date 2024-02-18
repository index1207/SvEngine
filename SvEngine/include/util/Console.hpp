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
    Log, // white
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
    static void Log(std::wstring category, LogType type, std::wstring message);
private:
    static void LogDisplay(std::wstring_view message);
    static void LogDebug(std::wstring_view message);
    static void LogError(std::wstring_view message);
private:
    static HANDLE s_handle;
};