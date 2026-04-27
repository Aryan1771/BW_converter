# BW Converter

BW Converter is a small C++ command-line utility that converts images into a custom grayscale `.bw` binary format and converts `.bw` files back into standard image files.

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

The converter writes a compact header followed by raw grayscale pixel data.

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

Install OpenCV and compile the source file with your C++ compiler. Example with `g++`:

```powershell
g++ bw_convertor.cpp -o bwtool.exe `pkg-config --cflags --libs opencv4`
```

On Windows, configure your compiler or IDE with the correct OpenCV include and library paths.

## Usage

Encode an image:

```powershell
.\bwtool.exe encode input.jpg output.bw
```

Decode a `.bw` file:

```powershell
.\bwtool.exe decode output.bw restored.png
```

## Notes

The current implementation supports single-image grayscale conversion. The header includes a `frames` field so the format can be extended later for video or multi-frame data.

## License

This repository is licensed under the GPL-3.0 license. See `LICENSE` for details.
