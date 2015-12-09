#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2\imgproc\imgproc_c.h>
#include <iostream>

using namespace cv;
using namespace std;

class Detector
{
public:
	Mat background;
	int umbral;
private:
	RNG rng;
	Mat convert(Mat input);
public:
	Detector(Mat background);
	~Detector();
	Mat getMask(Mat frame);
	Rect getBounds(Mat mask);
	void detectCamcool(VideoCapture cap);
};

