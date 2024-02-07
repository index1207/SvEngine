//
// Created by han93 on 2023-12-17.
//
#include "pch.h"

#include "util/Console.hpp"
#include "Windows.h"

using namespace sv;

HANDLE Console::m_consoleHandle;

void sv::Console::SetOutputEncoding()
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
}
    