#include "Detector.h"


Detector::Detector(Mat background) :rng(12345)
{
	this->background = background;
}


Detector::~Detector()
{
}
void Detector::detectCamcool(VideoCapture cap){
	{
		Ptr<BackgroundSubtractorMOG2>fondo = createBackgroundSubtractorMOG2();
		fondo->setNMixtures(3);
		fondo->setDetectShadows(true);
		Mat frame;
		Mat frontMask;
		Mat fondoMat;
		Mat edges;
		Mat flipRes;
		Mat endImage;
		namedWindow("Deteccion de gestos", 1);
		namedWindow("fondo", 1);
		namedWindow("mascara frontal", 1);

		int fondoFrames = 500;
		while (true)
		{
			cap >> frame; // get a new frame from camera
			cvtColor(frame, edges, CV_BGR2GRAY);
			GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
			//Update the current background model and get the foreground
			if (fondoFrames>0)
			{
				fondo->apply(edges, frontMask);
				fondoFrames--;
			}
			else
			{
				fondo->apply(edges, frontMask, 0);
			}
			fondo->getBackgroundImage(fondoMat);
			Canny(edges, edges, 0, 30, 3);
			//flip(edges, flipRes, 1);
			//edges.copyTo(endImage, frontMask);
			bitwise_and(edges, frontMask, endImage);
			imshow("Deteccion de gestos", endImage);
			imshow("fondo", fondoMat);
			imshow("mascara frontal", frontMask);
			if (waitKey(30) >= 0) break;
		}
	}
}
Mat Detector::getMask(Mat frame){
		Mat fondo = convert(this->background);
		Mat frontMask;
		Mat frame_converted;
		frame_converted = convert(frame);
		absdiff(fondo,frame_converted, frontMask);
		threshold(frontMask, frontMask, umbral, 255, cv::THRESH_BINARY);
		return frontMask;
}
Mat Detector::convert(Mat input){
	Mat output;
	cvtColor(input, output, CV_BGR2GRAY);
	GaussianBlur(output, output, Size(7, 7), 1.5, 1.5);
	return output;
}
Rect Detector::getBounds(Mat mask){
		return boundingRect(mask);
}