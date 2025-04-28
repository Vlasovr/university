#include "KeyVerifier.h"
#include "FileError.h"
#include "windows.h"
#include <fstream>
#include <iostream>


uint32_t KeyVerifier::getHash(const std::string& data) {
    uint32_t hash = START_HASH;
    constexpr uint32_t prime = PRIME;
    constexpr uint32_t mask = MASK;

    for (unsigned char byte : data) {
        hash ^= byte;
        hash *= prime;
        hash ^= (hash >> OFFSET);
        hash *= prime;
        hash &= mask;
    }

    return hash;
}

bool KeyVerifier::isRemovableDrive(const std::string& drivePath) {
    UINT driveType = GetDriveTypeA(drivePath.c_str());
    return driveType == DRIVE_REMOVABLE;
}

bool KeyVerifier::verify() {
    DWORD drives = GetLogicalDrives();
    if (drives == 0) {
        return false;
    }

    for (char drive = 'A'; drive <= 'Z'; ++drive) {
        if (drives & (1 << (drive - 'A'))) {
            std::string drivePath = std::string(1, drive) + ":\\";
            if (!isRemovableDrive(drivePath)) {
                continue;
            }

            std::ifstream file(drivePath + KEY_FILENAME, std::ios::binary);
            if (!file.is_open()) {
                continue;
            }

            std::string fileContent((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
            file.close();
            // std::cout << getHash(fileContent) << std::endl;
            // std::cout << fileContent.size() << std::endl;
            if (fileContent.size() == KEY_SIZE && getHash(fileContent) == KEY_HASH) {
                return true;
            }
        }
    }

    return false;
}
