//
// Created by han93 on 2023-12-17.
//

#pragma once

#include <string>

enum class LogType {
    Display, // white
    Debug,   // green
    Error    // red
};

class Console {
public:
    static void Log(std::string message, LogType type = LogType::Display);
};