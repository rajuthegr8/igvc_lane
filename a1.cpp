#include "opencv2/opencv.hpp"
#include<stdlib.h>
#include<limits.h>
#include<queue>
#include <sstream>
using namespace std;
using namespace cv;

Mat frame;//storing image on which computations are performed
Mat roi;//for storing the region of interest

void ROI()
{
    Mat temp(((9*frame.rows)/13), (frame.cols), CV_8UC3, Scalar(0,0,0));
    for (int i = 0; i < (9*frame.rows)/13; ++i)
    {
    	for (int j = 0; j < (frame.cols); ++j)
    	{


    		temp.at<Vec3b>(i,j)[0]=frame.at<Vec3b>(((2.8*frame.rows)/13)+i,j)[0];
    		temp.at<Vec3b>(i,j)[1]=frame.at<Vec3b>(((2.8*frame.rows)/13)+i,j)[1];
    		temp.at<Vec3b>(i,j)[2]=frame.at<Vec3b>(((2.8*frame.rows)/13)+i,j)[2];
    		
    	}

    }
    roi=temp.clone();
    
}
int main()
{
	VideoCapture vid("vid.mp4");
	namedWindow("ROI",WINDOW_NORMAL);
	namedWindow("Original",WINDOW_NORMAL);
	while(true)
	{
		vid>>frame;
		imshow("Original",frame);
		ROI();
		imshow("ROI",roi);
	    
	    waitKey(50);
	}
	waitKey(0);
	return 0;
}