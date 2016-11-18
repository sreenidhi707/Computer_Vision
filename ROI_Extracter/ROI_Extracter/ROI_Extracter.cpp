// ROI_Extracter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

bool leftDown = false, leftup = false;
Mat img;
Point cor1, cor2;
Rect box;
uint32_t count = 0;


void mouse_call(int event, int x, int y, int, void*)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		leftDown = true;
		cor1.x = x;
		cor1.y = y;
		cout << "Corner 1: " << cor1 << endl;

	}
	if (event == EVENT_LBUTTONUP)
	{
		if (abs(x - cor1.x)>20 && abs(y - cor1.y)>20) //checking whether the region is too small
		{
			leftup = true;
			cor2.x = x;
			cor2.y = y;
			cout << "Corner 2: " << cor2 << endl;
		}
		else
		{
			cout << "Select a region more than 20 pixels" << endl;
		}
	}

	if (leftDown == true && leftup == false) //when the left button is down
	{
		Point pt;
		pt.x = x;
		pt.y = y;
		Mat temp_img = img.clone();
		rectangle(temp_img, cor1, pt, Scalar(0, 0, 255)); //drawing a rectangle continuously
		imshow("Original", temp_img);

	}
	if (leftDown == true && leftup == true) //when the selection is done
	{

		box.width = abs(cor1.x - cor2.x);
		box.height = abs(cor1.y - cor2.y);
		box.x = min(cor1.x, cor2.x);
		box.y = min(cor1.y, cor2.y);
		Mat crop(img, box); //Selecting a ROI(region of interest) from the original pic
		namedWindow("Cropped Image");
		//imshow("Cropped Image", crop); //showing the cropped image
		
		string file_name = "C:/Users/sanand2/Dropbox/MyProjects/Computer_Vision/ROI_Extracter/ROI_Extracter/ROI_" + to_string(::count++) + ".jpg";
		imwrite(file_name, crop);
		leftDown = false;
		leftup = false;

	}


}

int main()
{
	img = imread("C:/Users/sanand2/Dropbox/MyProjects/OpenCV/OpencvTest/mcway.JPG", IMREAD_GRAYSCALE);
	namedWindow("Original");
	imshow("Original", img);

	//cvNamedWindow("Original", CV_WINDOW_NORMAL);
	//cvSetWindowProperty("Original", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	//imshow("Original", img);

	setMouseCallback("Original", mouse_call); //setting the mouse callback for selecting the region with mouse

	while (char(waitKey(1) != 'q')) //waiting for the 'q' key to finish the execution
	{

	}
	return 0;
}

