// ROI_Extracter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "opencv2\opencv.hpp"
#include <iostream>
#include "dataset.h"

using namespace cv;
using namespace std;

bool leftDown = false, leftup = false;
Mat img;
Point cor1, cor2;
Rect box;
uint32_t count = 0;

struct mouse_data
{
	string file_name;
};

void mouse_call(int event, int x, int y, int, void* data)
{
	mouse_data* mdata = (mouse_data*)data;

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
		Mat cropped(img, box); //Selecting a ROI(region of interest) from the original pic
		Mat cropped_resized;
		resize(cropped, cropped_resized, cv::Size(100, 100));
		
		string roi_file_name = output_folder + mdata->file_name + to_string(::count++) + ".jpg";
		imwrite(roi_file_name, cropped_resized);
		leftDown = false;
		leftup = false;
	}
}

void process_image(string file_name)
{
	string file_path = dataset_folder + file_name;
	img = imread(file_path, IMREAD_GRAYSCALE);
	namedWindow("Original");
	imshow("Original", img);

	mouse_data data;
	data.file_name = file_name;

	setMouseCallback("Original", mouse_call, (void*)&data); //setting the mouse callback for selecting the region with mouse

	while (char(waitKey(1) != 'q')) //waiting for the 'q' key to finish the execution
	{

	}
}

int main()
{

	for (size_t i = 0; i < NUMBER_OF_FILES; i++)
	{
		process_image(file_names[i]);
	}

	return 0;
}

