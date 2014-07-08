/*
 * Processor.cpp
 */

#include "Processor.h"
#include <sys/stat.h>
using namespace cv;

Processor::Processor()
{
}

Processor::~Processor()
{
  // TODO Auto-generated destructor stub
}

void Processor::processImage(int input_idx, image_pool* pool, int feature_type)
{
	// Get image from pool
	Mat greyimage = pool->getGrey(input_idx); // grayscale
	Mat img = pool->getImage(input_idx); // BGR

	// Return if no image is found
	if (img.empty() || greyimage.empty())
    	return;
    
    // Detect and draw Canny edges
    if (feature_type == DETECT_CANNY) {
		// Perform Canny edge detection
		Mat greyimagecanny;
		Canny(greyimage, greyimagecanny, 10, 100);
		
		// Copy edge image into output
		cvtColor(greyimagecanny, img, CV_GRAY2RGB); // BGR
		greyimagecanny.copyTo(greyimage); // grayscale
	}
	
	// Detect and draw strong lines
	else if (feature_type == DETECT_HOUGH_LINES) {
		// Resize to half resolution for faster processing
		Mat greyimagesmall;
		int resizefactor = 2;
		resize(greyimage, greyimagesmall, Size(greyimage.cols/resizefactor, greyimage.rows/resizefactor));
	
		// Perform Canny edge detection
		Mat greyimagesmallcanny;
		Canny(greyimagesmall, greyimagesmallcanny, 20, 200);
		
		// Detect lines by Hough transform and draw
		vector<Vec4i> lines;
		HoughLinesP(greyimagesmallcanny, lines, 1, CV_PI/180, 80, 30, 10);
		Point offset(-resizefactor, -resizefactor);
		for (int n = 0; n < lines.size(); n++) {
			Point pt1(resizefactor*lines[n][0], resizefactor*lines[n][1]);
			Point pt2(resizefactor*lines[n][2], resizefactor*lines[n][3]);
			line(img, pt1 + offset, pt2 + offset, Scalar(0,0,0), 3, 8);
			line(img, pt1, pt2, Scalar(255,255,0), 3, 8);
		} 
	}
	
	// Detect and draw strong circles
	else if (feature_type == DETECT_HOUGH_CIRCLES) {
		// Resize to half resolution for faster processing
		Mat greyimagesmall;
		int resizefactor = 2;
		resize(greyimage, greyimagesmall, Size(greyimage.cols/resizefactor, greyimage.rows/resizefactor));
	
		// Smooth the image so that not too many circles are detected
		GaussianBlur(greyimagesmall, greyimagesmall, Size(5,5), 2, 2);
		
		// Detect circles by Hough transform and draw
		vector<Vec3f> circles;
		HoughCircles(greyimagesmall, circles, CV_HOUGH_GRADIENT, 2, greyimagesmall.rows/8, 200, 100, 
			greyimagesmall.cols/25, greyimagesmall.cols/6);
		Point offset(-resizefactor, -resizefactor);
		for (int n = 0; n < circles.size(); n++) {
			Point center(cvRound(resizefactor*circles[n][0]), cvRound(resizefactor*circles[n][1]));
			int radius = cvRound(resizefactor*circles[n][2]);
			
			// Draw circle center
			circle(img, center + offset, 3, Scalar(0,0,0), -1, 8, 0);
			circle(img, center, 3, Scalar(255,255,0), -1, 8, 0);
			
			// Draw circle outline
			circle(img, center + offset, radius, Scalar(0,0,0), 3, 8, 0);
			circle(img, center, radius, Scalar(255,255,0), 3, 8, 0);
		}
	}
}