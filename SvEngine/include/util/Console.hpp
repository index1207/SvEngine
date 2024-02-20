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
    static void Log(String category, LogType type, String message);
private:
    static void LogDisplay(String message);
    static void LogDebug(String message);
    static void LogError(String message);
private:
    static HANDLE s_handle;
};