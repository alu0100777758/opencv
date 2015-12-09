#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <iostream>
#include "Detector.h"

using namespace cv;
using namespace std;

const  string defaultImage = "test.png";
const unsigned int fps = 30;
const unsigned int delayBetweeenImages = 1 / fps * 1000;
string calcularGesto(Rect bounds){
	if (bounds.height > 0 && bounds.width > 0){
		double altura = bounds.height;
		double tasa = altura / bounds.width;
		cout << tasa << endl;
		if (tasa < 1.10){
			return "5 dedos";
		}
		else if (tasa < 2.40 && tasa > 2.25){
			return "4 dedos";
		}
		else if (tasa > 1.20 && tasa < 1.38){
			return "3 dedos";
		}
		else if (tasa > 2 && tasa < 2.1){
			return "2 dedos";
		}
		else if (tasa > 1.9 && tasa < 2){
			return "2 dedos";
		}
	}
	return "desconocido";
}
int main(int argc, char** argv)
{
	VideoCapture cap(0); 
	if (!cap.isOpened()){  

		return -1;
		cerr << "no se ha podido acceder a la camara" << endl;
	}
	Mat backFrame;
	cout << "Se va a proceder a capturar el fondo, por favor pulse cualquier tecla cuando esté listo" << endl;
	while (true){
		cap >> backFrame;
		imshow("fondo",backFrame);
		if (waitKey(30) >= 0) break;
	}
	Detector det(backFrame);
	cout << "Captura realizada" << endl;
	det.umbral = 30;

	namedWindow("mascara", 1);
	namedWindow("Detection", 1);
	Mat mask;
	Mat frame;
	RNG rng(12345);
	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	while (true){
		cap >> frame;
		mask = det.getMask(frame);
		if (waitKey(30) >= 0) break;
		imshow("Máscara", mask);
		Rect bounds = det.getBounds(mask);
		rectangle(frame, bounds.tl(), bounds.br(), color, 2, 8, 0);
		putText(frame, calcularGesto(bounds), cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);
		imshow("Detection", frame);
	}
	return 0;
}