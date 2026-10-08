#include "../bw_header.h"
#include <cassert>

int main() {
    BWHeader header{};
    header.width = 2;
    header.height = 3;
    header.datasize = 6;
    assert(valid_image_header(header));
    header.datasize = 7;
    assert(!valid_image_header(header));
    header.datasize = 6;
    header.magic[0] = 'X';
    assert(!valid_image_header(header));
    header.magic[0] = 'B';
    header.version = 2;
    assert(!valid_image_header(header));
    header.version = 1;
    header.width = 0;
    assert(!valid_image_header(header));
    header.width = 0xFFFFFFFF;
    header.height = 0xFFFFFFFF;
    assert(!valid_image_header(header));
    header.width = 10001;
    header.height = 10000;
    header.datasize = 100010000;
    assert(!valid_image_header(header));
    header.width = 2;
    header.height = 3;
    header.datasize = 6;
    header.frames = 2;
    assert(!valid_image_header(header));
    header.frames = 1;
    header.type = 1;
    assert(!valid_image_header(header));
}
