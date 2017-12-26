#pragma once

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;


class sharpnessCore {
	public :
		sharpnessCore();
		vector<string> findImageFiles(const string&, const string&);
		vector<Mat> makeImageToMat(vector<string>);
		vector<int> measureSharpnessBySobel(Mat);
		vector<vector<int> > measureSharpnessBySobel(vector<Mat>);

		vector<int> compareSharpness(vector<int>, vector<int>);
		vector<int> compareSharpness(vector<vector<int> >);


		Mat createImage(Mat, Mat, vector<int>);
		Mat createImage(vector<Mat>, vector<int>);
};