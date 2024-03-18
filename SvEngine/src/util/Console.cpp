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

void Console::Log(String category, LogType type, String message)
{
    switch (type)
    {
    case LogType::Log:
        LogDisplay(category, message);
        break;
    case LogType::Warning:
        LogWarning(category, message);
        break;
    case LogType::Debug:
        LogDebug(category, message);
        break;
    case LogType::Error:
        LogError(category, message);
        break;
    default:
        break;
    }
}

void Console::Print(ColorOperation color, String message, bool ln)
{
    if (ln) message += L'\n';
    std::cout << color << action::ToAnsiString(message);
}

void Console::LogDisplay(String category, String message)
{
    Print(hue::bright_white, std::format(TEXT("[{}][Log] {}"), category, message));
}

void Console::LogWarning(String category, String message)
{
    Print(hue::yellow, std::format(TEXT("[{}][Warning] {}"), category, message));
}

void Console::LogDebug(String category, String message)
{
    Print(hue::light_green, std::format(TEXT("[{}][Debug] {}"), category, message));
}

void Console::LogError(String category, String message)
{
    Print(hue::light_red, std::format(TEXT("[{}][Error] {}"), category, message));
}

void Console::Initialize() {
    s_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleOutputCP(CP_UTF8);

    setlocale(LC_ALL, "");
}
    