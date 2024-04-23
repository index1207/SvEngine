//
// Created by han93 on 2023-12-17.
//
#include "pch.h"

#include "util/Console.hpp"
#include "Windows.h"

HANDLE Console::s_handle;

void Console::SetOutputEncoding()
{
}

void Console::Log(String category, LogType type, String message)
{
    switch (type)
    {
    case LogType::Info:
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

void Console::Print(Color color, String message, bool ln)
{
    message += L'\n';

    SetConsoleTextAttribute(s_handle, color);
    WriteConsole(s_handle, message.c_str(), message.length(), nullptr, nullptr);
}

void Console::LogDisplay(String category, String message)
{
    Print(Color::White, std::format(TEXT("[{}][INFO] {}"), category, message));
}

void Console::LogWarning(String category, String message)
{
    Print(Color::Yellow, std::format(TEXT("[{}][WARNING] {}"), category, message));
}

void Console::LogDebug(String category, String message)
{
    Print(Color::Green, std::format(TEXT("[{}][DEBUG] {}"), category, message));
}

void Console::LogError(String category, String message)
{
    Print(Color::Red, std::format(TEXT("[{}][Error] {}"), category, message));
}

void Console::Initialize() {
    s_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleOutputCP(CP_UTF8);

    setlocale(LC_ALL, "");
}