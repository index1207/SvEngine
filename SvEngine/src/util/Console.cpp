//
// Created by han93 on 2023-12-17.
//
#include "pch.h"

#include "util/Console.hpp"
#include "Windows.h"

HANDLE Console::m_consoleHandle;

void Console::SetOutputEncoding()
{

}

void Console::Log(std::string_view message, LogType type) {
    std::osyncstream(std::cout) << message << std::endl;
}

void Console::Initialize() {
    #ifdef WIN32
    m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleOutputCP(CP_UTF8);
    #endif

    std::cout.imbue(std::locale("kor"));
}
    