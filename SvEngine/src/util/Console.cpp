//
// Created by han93 on 2023-12-17.
//
#include "pch.h"

#include "util/Console.hpp"
#include "Windows.h"

#include "color.hpp"

HANDLE Console::s_handle;

void Console::SetOutputEncoding()
{
}

void Console::Log(std::wstring category, LogType type, std::wstring message) {
    message = L"[" + category + L"] " + message;
    switch (type)
    {
    case LogType::Log:
        LogDisplay(message);
        break;
    case LogType::Debug:
        LogDebug(message);
        break;
    case LogType::Error:
        LogError(message);
        break;
    default:
        break;
    }
}

void Console::LogDisplay(std::wstring_view message)
{
    std::osyncstream(std::cout) << hue::bright_white;
    std::wosyncstream(std::wcout) << "[Log]" << message << std::endl;
}

void Console::LogDebug(std::wstring_view message)
{
    std::osyncstream(std::cout) << hue::light_green;
    std::wosyncstream(std::wcout) << "[Debug]" << message << std::endl;
}

void Console::LogError(std::wstring_view message)
{
    std::osyncstream(std::cout) << hue::light_red;
    std::wosyncstream(std::wcout) << "[Error]" << message << std::endl;
}

void Console::Initialize() {
    #ifdef WIN32
    s_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleOutputCP(CP_UTF8);
    #endif

    setlocale(LC_ALL, "");
}
    