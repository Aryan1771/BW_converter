#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>

struct BWHeader {
    char magic[4] = {'B','W','F','M'};
    uint8_t version = 1;
    uint8_t type;        // 0 image, 1 video
    uint32_t width;
    uint32_t height;
    uint32_t frames;
    uint32_t datasize;
};

void image_to_bw(std::string input, std::string output)
{
    cv::Mat img = cv::imread(input);
    if(img.empty()){
        std::cout<<"Error loading image\n";
        return;
    }

    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    BWHeader header;
    header.type = 0;
    header.width = gray.cols;
    header.height = gray.rows;
    header.frames = 1;
    header.datasize = gray.total();

    std::ofstream file(output, std::ios::binary);
    file.write((char*)&header, sizeof(header));
    file.write((char*)gray.data, header.datasize);

    file.close();
}

void bw_to_image(std::string input, std::string output)
{
    std::ifstream file(input, std::ios::binary);

    BWHeader header;
    file.read((char*)&header, sizeof(header));

    if(header.type != 0){
        std::cout<<"Not an image\n";
        return;
    }

    cv::Mat gray(header.height, header.width, CV_8UC1);
    file.read((char*)gray.data, header.datasize);

    cv::imwrite(output, gray);
}

int main(int argc, char** argv)
{
    if(argc < 4){
        std::cout<<"Usage:\n";
        std::cout<<"convert image -> bw: bwtool encode input.jpg output.bw\n";
        std::cout<<"convert bw -> image: bwtool decode input.bw output.png\n";
        return 0;
    }

    std::string mode = argv[1];

    if(mode == "encode")
        image_to_bw(argv[2], argv[3]);

    if(mode == "decode")
        bw_to_image(argv[2], argv[3]);
}