#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

#ifndef CONVERTER_H
#define CONVERTER_H

class Converter {
public:
	cv::Mat base64ToMat(const string& imageBase64);
	string matToBase64(const Mat& img);
	std::string base64_encode(uchar const* bytesToEncode, unsigned int inLen);
	std::string base64_decode(std::string const& encodedString);
};

#endif
