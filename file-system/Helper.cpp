//
// Created by Oguzhan Agkus on 17.05.2022.
//

#include "Helper.h"

void log(const std::string &message) {
    std::cout << message << std::endl;
}

void exitWithError(const std::string &errorMessage, int errorCode) {
    std::cout << errorMessage << std::endl;
    if (errorCode != -1)
        std::cout << "Error: " << strerror(errorCode) << std::endl;
    exit(errorCode);
}

uint32_t divide_ceil(uint32_t a, uint32_t b) {
    float temp = (float) a / (float) b;
    temp = std::ceil(temp);
    return (uint32_t) temp;
}
