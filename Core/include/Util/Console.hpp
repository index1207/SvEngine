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
    Info, // white
    Warning,
    Debug,   // green
    Error    // red
};

enum Color : WORD
{
    Black,    
    DarkBlue, 
    DarkGreen,
    LightBlue,
    DarkRed,
    Magenta,
    Orange,
    LightGray,
    Gray,
    Blue,
    Green,
    Cyan,
    Red,
    Pink,
    Yellow,
    White
};

enum EncodingType {
    ANSI,
    UTF8,
};

class SVENGINE_API Console {
    friend class Engine;

    static void Initialize();
public:
    static void SetOutputEncoding();
public:
    static void Print(String category, LogType type, String message);
    static void Print(Color color, String message);
    static void Log(String category, String message);
    static void Warning(String category, String message);
    static void Debug(String category, String message);
    static void Error(String category, String message);
private:
    static HANDLE s_handle;
};