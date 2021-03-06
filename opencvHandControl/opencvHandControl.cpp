// opencvHandControl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BlobDetector.h"
#include "MouseTracker.h"

using namespace cv;
using namespace std;


void MouseClick(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONDOWN)
	{
		Mat* img = (Mat*)userdata;
		cv::Vec3b& pixel = img->at<cv::Vec3b>(y, x);
		BlobDetector::convertPixel2HLS(pixel,*img);
		BlobDetector::H = pixel[0];
		BlobDetector::L = pixel[1];
		BlobDetector::S = pixel[2];
		std::printf("H = %d L = %d S = %d\n", pixel[0], pixel[1], pixel[2]);
	}
}

int main(int argc, char** argv)
{
	std::printf("******************************DeVeLoPeD bY Mk Anis******************************\n");
	//asks wether to show the video feed or not
	bool showVid = false;
	char confirm;
	printf("show video feed or not ? (y/n)\n");
	cin >> confirm;
	if (confirm == 'y' || confirm == 'Y') showVid = true;

	VideoCapture stream(0);   //0 is the id of video device.0 if you have only one camera.

	if (!stream.isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
	}
	Mat cameraFrame;
	stream.read(cameraFrame);

	//initialize mouse tracker
	MouseTracker::setImgDim(cameraFrame.cols, cameraFrame.rows);
	MouseTracker::initalize();

	/*cv::Vec3b pixel[1][1] = { {cv::Vec3b(255,0,0)} };
	cv::Mat img(1, 1, cameraFrame.type(), pixel);
	cv::Mat temp;
	cvtColor(img, temp, cv::COLOR_BGR2HLS);
	imshow("Test", temp);*/

	while (true) {
		stream.read(cameraFrame);
		if (showVid) {
			namedWindow("cam");
			setMouseCallback("cam", MouseClick, &cameraFrame);
		}

		// Set up the detector with default parameters.
		Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create();

		// Detect blobs.
		const std::vector<std::pair<cv::KeyPoint, int> >& keypoints = BlobDetector::detect(cameraFrame);
		
		//synch the mouse with the mvt
		MouseTracker::syncMousePos(keypoints);

		// Draw detected blobs as red circles.
		// DrawMatchesFlags::sDRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
		Mat im_with_keypoints;
		//drawKeypoints(cameraFrame, keypoints, im_with_keypoints, Scalar(0, 255, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		if (showVid)
			imshow("cam", cameraFrame);
		if (waitKey(30) >= 0)
			break;
	}
	return 0;
}