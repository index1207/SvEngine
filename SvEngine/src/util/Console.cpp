//
// Created by han93 on 2023-12-17.
//
#include "pch.h"

#include "util/Console.hpp"
#include "Windows.h"

#include "color.hpp"

HANDLE Console::m_consoleHandle;

void Console::SetOutputEncoding()
{
}

void Console::Log(std::string_view message, LogType type) {
    switch (type)
    {
    case LogType::Display:
        LogDisplay(message);
        break;
    case LogType::Debug:
        LogDebug(message);
        break;
    case LogType::Error:
        LogCategory(message);
        break;
    default:
        break;
    }
}

void Console::LogDisplay(std::string_view message)
{
    std::osyncstream(std::cout) << hue::bright_white << message << std::endl;
}

void Console::LogDebug(std::string_view message)
{
    std::osyncstream(std::cout) << hue::light_green << message << std::endl;
}

void Console::LogCategory(std::string_view message)
{
    std::osyncstream(std::cout) << hue::light_red << message << std::endl;
}

void Console::Initialize() {
    #ifdef WIN32
    m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleOutputCP(CP_UTF8);
    #endif

    std::cout.imbue(std::locale("kor"));
}
    