// CIFAR_Helper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include <vector>

#include "opencv2\opencv.hpp"

using namespace cv;
using namespace std;

const string cifar_folder = "C://Users//sanand2//Google_Drive//Projects//datasets//cifar-10-batches-bin//data_batch_1.bin";
const uint32_t NUM_IMAGES_PER_BIN = 10000;
const uint32_t NUM_ROWS_IN_IMAGE = 32;
const uint32_t NUM_COLS_IN_IMAGE = 32;
const uint32_t BYTES_PER_IMAGE = 3072 + 1;
const string labels[10] = {"airplane", "automobile", "bird", "cat", "deer", "dog", "frog", "horse", "ship", "truck"};

int main()
{
	ifstream input(cifar_folder.c_str(), std::ios::binary);
	// copies all data into buffer
	vector<char> buffer((istreambuf_iterator<char>(input)),(istreambuf_iterator<char>()));
	input.close();

	uint32_t data_offset = 0;

	for (size_t img = 0; img < 1; img++)
	{
		cout << "Processing image " << img << endl;
		uint32_t offset = img * BYTES_PER_IMAGE;
		
		vector<Mat> image_channels;
		uint8_t label = 0;
		label = uchar(buffer.at(offset));
		offset++;
		for (size_t channel = 0; channel < 3; channel++)
		{
			Mat image = Mat::zeros(NUM_ROWS_IN_IMAGE, NUM_COLS_IN_IMAGE, CV_8U);
			int sz = image.channels();
			cout << sz << endl;
			for (size_t row = 0; row < NUM_ROWS_IN_IMAGE; row++)
			{
				for (size_t col = 0; col < NUM_COLS_IN_IMAGE; col++)
				{
					image.at<uchar>(row, col) = uchar(buffer.at(offset + row * NUM_COLS_IN_IMAGE + col));
				}
			}
			image_channels.push_back(image);
		}
		//Swap RGB to GBR
		Mat red = image_channels[0];
		image_channels[0] = image_channels[2];
		image_channels[2] = red;

		Mat final_image_BGR;// = Mat::zeros(NUM_ROWS_IN_IMAGE, NUM_COLS_IN_IMAGE, CV_8UC3);;
		merge(image_channels, final_image_BGR);
		cout << "final_image_BGR type" << final_image_BGR.type() << endl;
		int sz = final_image_BGR.channels();
		cout << sz << endl;
		Mat color_converted;
		//cvtColor(final_image_BGR, color_converted, CV_BGR2HSV);

		Mat z = Mat::zeros(NUM_ROWS_IN_IMAGE, NUM_COLS_IN_IMAGE, CV_8UC1);
		{
			vector<Mat> channels;
			Mat fin_img;
			channels.push_back(image_channels[0]);
			channels.push_back(image_channels[1]);
			channels.push_back(image_channels[2]);
			merge(channels, fin_img);
			namedWindow("R", 1); imshow("R", fin_img);

			cout << "Red Pixel values" << endl;
			for (size_t row = 0; row < NUM_ROWS_IN_IMAGE; row++)
			{
				for (size_t col = 0; col < NUM_COLS_IN_IMAGE; col++)
				{
					printf(" %3d  ", image_channels[2].at<uchar>(row, col));
				} 
				printf("\n");
			}
		}
		//{
		//	vector<Mat> channels;
		//	Mat fin_img;
		//	channels.push_back(z);
		//	channels.push_back(image_channels[1]);
		//	channels.push_back(z);
		//	merge(channels, fin_img);
		//	namedWindow("G", 1); imshow("G", fin_img);

		//	cout << "Green Pixel values" << endl;
		//	for (size_t row = 0; row < NUM_ROWS_IN_IMAGE; row++)
		//	{
		//		for (size_t col = 0; col < NUM_COLS_IN_IMAGE; col++)
		//		{
		//			printf(" %3d  ", image_channels[1].at<uchar>(row, col));
		//		}
		//		printf("\n");
		//	}
		//}
		//{
		//	vector<Mat> channels;
		//	Mat fin_img;
		//	channels.push_back(image_channels[0]);
		//	channels.push_back(z);
		//	channels.push_back(z);
		//	merge(channels, fin_img);
		//	namedWindow("B", 1); imshow("B", fin_img);

		//	cout << "Blue Pixel values" << endl;
		//	for (size_t row = 0; row < NUM_ROWS_IN_IMAGE; row++)
		//	{
		//		for (size_t col = 0; col < NUM_COLS_IN_IMAGE; col++)
		//		{
		//			printf(" %3d  ", image_channels[0].at<uchar>(row, col));
		//		}
		//		printf("\n");
		//	}
		//}
		
		imshow("Showing Image", final_image_BGR);
		string image_name = labels[label] + "_" + to_string(img) + ".jpg";
		imwrite(image_name, final_image_BGR);
	}
	waitKey(0);
	return 0;
}

