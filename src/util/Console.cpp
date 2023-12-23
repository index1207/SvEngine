//
// Created by han93 on 2023-12-17.
//

#include "util/Console.hpp"
#include <windows.h>

using namespace sv;

void Console::Log(std::string message, LogType type) {

}

void Console::Initialize() {
    #ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
}
