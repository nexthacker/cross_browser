#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <random>

constexpr double PI = 3.14159265358979323846;
static constexpr double degreesToRadians(double degrees) {
  return degrees * PI / 180.0;
}

std::string type2str(int type) {
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch (depth) {
  case CV_8U:
    r = "8U";
    break;
  case CV_8S:
    r = "8S";
    break;
  case CV_16U:
    r = "16U";
    break;
  case CV_16S:
    r = "16S";
    break;
  case CV_32S:
    r = "32S";
    break;
  case CV_32F:
    r = "32F";
    break;
  case CV_64F:
    r = "64F";
    break;
  default:
    r = "User";
    break;
  }

  r += "C";
  r += (chans + '0');

  return r;
}

int main() {
  cv::Mat out(1080, 1920, CV_16UC3);
  constexpr double range = ((1<<16) - 1)/2.0;
  constexpr double nu = 2 * PI / 1920;
  for (int j = 0; j < 1080; ++j) {
    uint16_t *dst = out.ptr<uint16_t>(j);
    for (int i = 0; i < 1920; ++i) {
      const int r = 0*cv::saturate_cast<uint16_t>(
          std::sin(nu * i + degreesToRadians(0)) * range + range);
      const int g = cv::saturate_cast<uint16_t>(
          std::sin(nu * i + degreesToRadians(120)) * range + range);
      const int b = 0*cv::saturate_cast<uint16_t>(
          std::sin(nu * i + degreesToRadians(240)) * range + range);
      dst[3 * i + 0] = b;
      dst[3 * i + 1] = g;
      dst[3 * i + 2] = r;
    }
  }

  cv::imwrite("rainbow.png", out);

  cv::Mat test (256, 256, CV_16UC3);
  std::random_device seed;
  std::mt19937_64 gen (seed());
  std::uniform_int_distribution<> dist (0, 1<<16);
  for (int j = 0; j < test.rows; ++j) {
    uint16_t * dst = test.ptr<uint16_t>(j);
    for (int i = 0; i < test.cols; ++i) {
      dst[3*i + 0] = dist(gen);
      dst[3*i + 1] = dist(gen);
      dst[3*i + 2] = dist(gen);
    }
  }

  cvNamedWindow("Out", CV_WINDOW_NORMAL);
  cv::imshow("Out", test);
  cv::waitKey(0);
  cv::imwrite("test.png", test);

  cv::Mat in = cv::imread("test.png", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_COLOR);
  std::cout << type2str(in.type()) << "  " << type2str(test.type()) << std::endl;
}