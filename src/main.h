#include <opencv2/core.hpp>

int main();
void draw_rotated_text(cv::Mat& textOverlay, double angleRotate, cv::Mat& image);
void process_image(const std::string imageIn, const std::string outFile);
void create_png_from_binary(const std::string& binary, const std::string& fileName);
void resize_image(cv::Mat& image);
bool check_photo_allowed(const std::string& image_file);
double degrees(const double& radians);
void remove_trash_file(const std::string& filename);
int random_int(const int& startInt, const int& endInt);