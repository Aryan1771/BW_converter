# BW Converter

BW Converter is a C++ command-line utility that converts images into a custom grayscale `.bw` binary format and converts `.bw` files back into standard image files.

## Features

- Encodes an input image as an 8-bit grayscale binary file
- Decodes the custom `.bw` format back into an image
- Stores metadata such as file type, width, height, frame count, and data size
- Uses OpenCV for image loading, grayscale conversion, and image writing
- Simple command-line interface for encode/decode workflows

## Tech Stack

- C++
- OpenCV
- Binary file I/O

## File Format

The converter writes the in-memory C++ header followed by raw grayscale pixel data. The layout depends on compiler padding and machine byte order; this is an experimental format, not a portable interchange specification.

```cpp
struct BWHeader {
    char magic[4] = {'B','W','F','M'};
    uint8_t version = 1;
    uint8_t type;        // 0 image, 1 video
    uint32_t width;
    uint32_t height;
    uint32_t frames;
    uint32_t datasize;
};
```

## Build

Install OpenCV and compile the source file with your C++ compiler. On Linux with OpenCV development files and `pkg-config`:

```bash
g++ -std=c++11 bw_convertor.cpp -o bwtool $(pkg-config --cflags --libs opencv4)
```

On Windows, configure your compiler or IDE with the correct OpenCV include and library paths.

## Usage

Encode an image:

```bash
./bwtool encode input.jpg output.bw
```

Decode a `.bw` file:

```bash
./bwtool decode output.bw restored.png
```

## Validation

The header regression test does not require OpenCV:

```bash
g++ -std=c++11 tests/header_test.cpp -o header_test
./header_test
```

It covers valid headers, mismatched sizes, invalid magic/version/type, zero dimensions, overflow-sized dimensions, and the image-size limit. Image encoding/decoding still needs OpenCV integration testing.

## Limitations

The decoder checks magic bytes, version, image type, dimensions, frame count, and exact payload length before allocating the image buffer. Images are limited to 100 million pixels. Invalid files, failed reads/writes, and unsupported modes return a nonzero exit code.

The current implementation supports single-image grayscale conversion. The header includes a `frames` field so the format can be extended later for video or multi-frame data.

## License

This repository is licensed under the GPL-3.0 license. See `LICENSE` for details.
