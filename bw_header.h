#pragma once

#include <cstdint>
#include <cstring>
#include <limits>

struct BWHeader {
    char magic[4] = {'B', 'W', 'F', 'M'};
    uint8_t version = 1;
    uint8_t type = 0;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t frames = 1;
    uint32_t datasize = 0;
};

inline bool valid_image_header(const BWHeader& header) {
    const uint64_t pixels = static_cast<uint64_t>(header.width) * header.height;
    return std::memcmp(header.magic, "BWFM", 4) == 0 && header.version == 1 &&
        header.type == 0 && header.frames == 1 && header.width > 0 && header.height > 0 &&
        header.width <= static_cast<uint32_t>(std::numeric_limits<int>::max()) &&
        header.height <= static_cast<uint32_t>(std::numeric_limits<int>::max()) &&
        pixels <= 100000000 && pixels == header.datasize;
}
