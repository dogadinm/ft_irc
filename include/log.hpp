#pragma once

#include <iostream>
#include <string>

#include <cstdlib>
#include <cstdio>
#include <time.h>


static inline void log(const std::string& message) {
    char buffer[80];
    time_t rawtime = time(NULL);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", localtime(&rawtime));

    std::cout << "\033[0;34m[" << buffer << "]\033[0m " << message << std::endl;
}