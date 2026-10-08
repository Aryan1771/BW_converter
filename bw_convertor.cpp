#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "bw_header.h"


void image_to_bw(std::string input, std::string output)
{
    cv::Mat img = cv::imread(input);
    if(img.empty()){
        std::cout<<"Error loading image\n";
        throw std::runtime_error("Cannot load input image");
    }

    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    BWHeader header{};
    header.type = 0;
    header.width = gray.cols;
    header.height = gray.rows;
    header.frames = 1;
    if (gray.total() > 100000000) {
        throw std::runtime_error("Image exceeds the 100 million pixel limit");
    }
    header.datasize = static_cast<uint32_t>(gray.total());

    std::ofstream file(output, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open output file");
    file.write((char*)&header, sizeof(header));
    file.write((char*)gray.data, header.datasize);
    if (!file) throw std::runtime_error("Cannot write output file");

    file.close();
}

void bw_to_image(std::string input, std::string output)
{
    std::ifstream file(input, std::ios::binary);

    BWHeader header{};
    if (!file.read(reinterpret_cast<char*>(&header), sizeof(header))) {
        throw std::runtime_error("Missing or truncated BW header");
    }
    const uint64_t pixels = static_cast<uint64_t>(header.width) * header.height;
    if (!valid_image_header(header)) {
        throw std::runtime_error("Invalid or unsupported BW image header");
    }
    const auto payload_start = file.tellg();
    file.seekg(0, std::ios::end);
    if (file.tellg() - payload_start != static_cast<std::streamoff>(pixels)) {
        throw std::runtime_error("BW payload length does not match the header");
    }
    file.seekg(payload_start);
    cv::Mat gray(header.height, header.width, CV_8UC1);
    if (!file.read(reinterpret_cast<char*>(gray.data), header.datasize)) {
        throw std::runtime_error("Cannot read BW image payload");
    }

    if (!cv::imwrite(output, gray)) throw std::runtime_error("Cannot write output image");
}

int main(int argc, char** argv)
{
    if(argc != 4){
        std::cout<<"Usage:\n";
        std::cout<<"convert image -> bw: bwtool encode input.jpg output.bw\n";
        std::cout<<"convert bw -> image: bwtool decode input.bw output.png\n";
        return 1;
    }

    std::string mode = argv[1];

    try {
        if (mode == "encode") image_to_bw(argv[2], argv[3]);
        else if (mode == "decode") bw_to_image(argv[2], argv[3]);
        else throw std::runtime_error("Unknown mode: use encode or decode");
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
