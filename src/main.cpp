#include <cstdlib>
#include <iostream>
#include <math.h>
#include <new>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "main.h"
#include "bot.h"
#include <fstream>
#include <ostream>
#include <filesystem>

int main() {
    launch_bot();
    return 0;
}

void process_image(const std::string imageIn, const std::string outFile) {
    std::cout<<"process_image call"<<std::endl;
    std::cout<<"Обработка картинки "<<imageIn<<std::endl;
    cv::Mat _image = cv::imread(imageIn);
    cv::Mat _resImage;

    resize_image(_image);

    if(!_image.data || _image.empty()) {
        std::cout<<"Неправильное имя файла или битая картинка."<<std::endl;
        return;
    }
    
    cv::Mat _textOverlay = cv::Mat::zeros(1080, 1080, _image.type());
    cv::Mat _bgTemp = cv::Mat::zeros(_image.rows, _image.cols, _image.type());
    _textOverlay.setTo(cv::Scalar(0, 0, 0, 255));
    cv::putText(_textOverlay, "ЛИКВИДИРОВАН", cv::Point(0, _textOverlay.cols/2), cv::FONT_HERSHEY_COMPLEX, 3.5, cv::Scalar(0, 0, 255, 360), 12);
    draw_rotated_text(_textOverlay, degrees(atan(static_cast<double>(_image.size().height) / static_cast<double>(_image.size().width))), _textOverlay);
    
    cv::resize(_textOverlay, _textOverlay, _image.size());
    cv::addWeighted(_image, 0.75, _bgTemp, 0.25, 0, _image);
    cv::add(_image, _textOverlay, _resImage);
    cv::imwrite(outFile, _resImage);
    return;
}

void draw_rotated_text(cv::Mat& textOverlay, double angleRotate, cv::Mat& image) {
    std::cout<<"draw_rotated_text call"<<std::endl;
    int len = std::max(textOverlay.cols, textOverlay.rows);
    cv::Point2f pt(len/2., len/2.3);
    cv::Mat r = cv::getRotationMatrix2D(pt, angleRotate, 1.0);

    cv::warpAffine(textOverlay, image, r, cv::Size(len, len));
}

void resize_image(cv::Mat& image) {
    std::cout<<"resize_image call"<<std::endl;
    double _width, _deltaWidth;
    if((3200. / image.size().width) <= 2) {
        _width = image.size().width, _deltaWidth = (3200. / _width);
    } else {
        _width = image.size().width, _deltaWidth = (1600. / _width);
    }
    cv::resize(image, image, cv::Mat::zeros(image.size().height * _deltaWidth, image.size().width * _deltaWidth, image.type()).size());
}

void create_png_from_binary(const std::string& binary, const std::string& fileName) {
    std::cout<<"create_png_from_binary call"<<std::endl;
    std::ofstream _pngFile;
    _pngFile.open(fileName);
    _pngFile<<binary;
}

bool check_photo_allowed(const std::string& image_file) {
    return std::filesystem::file_size(image_file) < 5120000 ? true : false; 
}

double degrees(const double& radians) {
    return radians * (180./M_PI);
}

void remove_trash_file(const std::string& filename) {
    std::filesystem::remove(filename);
    std::filesystem::remove("done_" + filename);
}

int random_int(const int& startInt, const int& endInt) {
    return std::rand() % (endInt - startInt + 1) + startInt;
}