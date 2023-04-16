//
// Created by Oguzhan Agkus on 17.05.2022.
//

#ifndef FILE_SYSTEM_HELPER_H
#define FILE_SYSTEM_HELPER_H

#include <cmath>
#include <cstring>
#include <iostream>

void log(const std::string &message);

void exitWithError(const std::string &errorMessage, int errorCode = -1);

uint32_t divide_ceil(uint32_t a, uint32_t b);

#endif //FILE_SYSTEM_HELPER_H
