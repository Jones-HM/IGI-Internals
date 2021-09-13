#pragma once
#include ".\Common.h"
/*
     * @brief Scan for a given byte pattern on a module
     *
     * @Param module    Base of the module to search
     * @Param signature IDA-style byte array pattern
     *
     * @Returns Address of the first occurence
     */

std::uint8_t* PatternScan(void* module, const char* signature)
{
    static auto pattern_to_byte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else {
                bytes.push_back(strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    auto dosHeader = (PIMAGE_DOS_HEADER)module;
    auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

    auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
    auto patternBytes = pattern_to_byte(signature);
    auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

    auto s = patternBytes.size();
    auto d = patternBytes.data();

    for (auto i = 0ul; i < sizeOfImage - s; ++i) {
        bool found = true;
        for (auto j = 0ul; j < s; ++j) {
            if (scanBytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }
        if (found) {
            return &scanBytes[i];
        }
    }
    return nullptr;
}


bool InitSigPatterns(std::string& sigErrorReason) {
    const char* gameLoadedSig = "25 73 2F 6F 62 6A 65 63 74 73 2E"; //Level Load Path
    const char* playerLoadedSig = "1E 00 00 00 24 BA ?? ??"; //Game FPS.
    const char* gameRunningSig = "02 00 00 00 00 00 00 00 58 53 68"; //Menu Manager
    bool sigStatus = true;

    auto moduleHandle = GetModuleHandle(NULL);
    auto gameLoadedSigAddr = PatternScan(moduleHandle, gameLoadedSig);
    auto playerLoadedSigAddr = PatternScan(moduleHandle, playerLoadedSig);
    auto gameRunningSigAddr = PatternScan(moduleHandle, gameRunningSig);

    #if _DEBUG
    LOG_DEBUG("GameLoaded Sig : %p", gameLoadedSigAddr);
    LOG_DEBUG("PlayerLoaded Sig : %p", playerLoadedSigAddr);
    LOG_DEBUG("GameRunning Sig : %p", gameRunningSigAddr);
    #endif

    if (gameLoadedSigAddr == nullptr) {
        sigErrorReason = "\nGame load could not be verified!";
        sigStatus = false;
    }

    if (playerLoadedSigAddr == nullptr) {
        sigErrorReason = "\nPlayer load could not be verified!";
        sigStatus = false;
    }

    if (gameRunningSigAddr != nullptr) {
        sigErrorReason = "\nGame Running could not be verified!";
        sigStatus = false;
    }
    return sigStatus;
}