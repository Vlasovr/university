#pragma once
#include <string>
#include <cstdint>
#define START_HASH 2166136261u
#define PRIME 16777619u
#define MASK 0xFFFFFFF
#define OFFSET 13

#define KEY_HASH 224187629
#define KEY_SIZE 128
#define KEY_FILENAME "kts_key.dat"

class KeyVerifier {
public:
    static bool verify();
private:
    static bool isRemovableDrive(const std::string& drivePath);
    static uint32_t getHash(const std::string& data);
};
