#include "stdafx.h"
#include "SharpnessCore.h"

#include <io.h>

static const int MASTERVALUE = 100;

sharpnessCore::sharpnessCore() {

	/*
	Mat imgMat1 = imread("C:\\Users\\Administrator\\Desktop\\image\\ER-217.bmp", IMREAD_GRAYSCALE);

	vector<int> img1 = measureSharpnessBySobel(imgMat1);

	Mat imgMat2 = imread("C:\\Users\\Administrator\\Desktop\\image\\ER-248.bmp", IMREAD_GRAYSCALE);

	vector<int> img2 = measureSharpnessBySobel(imgMat2);

	vector<int> sharpnessOrder = compareSharpness(img1, img2);

	Mat resultMat = createImage(imgMat1, imgMat2, sharpnessOrder);
	*/

	//find imageFiles
	vector<string> imageFileNames = findImageFiles("C:\\Users\\Administrator\\Desktop\\image\\", "*.bmp");

	//imageMat make
	vector<Mat> imageMats = makeImageToMat(imageFileNames);
	
	//measureSharpnessBySobel
	vector<vector<int> > sharpnessVector = measureSharpnessBySobel(imageMats);

	//compareSharpness
	vector<int> sharpnessOrder = compareSharpness(sharpnessVector);


	//createImage
	Mat resultMat = createImage(imageMats, sharpnessOrder);

	imshow("result", resultMat);

	waitKey(0);

}

vector<string> sharpnessCore::findImageFiles(const std::string& _path, const std::string& _filter) {

	// find all matched file in specified directory
	// [INPUT]
	//   >> const std::string& _path        Search path        ex) c:/directory/
	//   >> const std::string& _filter        Search filter    ex) *.exe or *.*
	// [RETURN]
	//   >> std::vector<std::string>        All matched file name & extension
	
	string searching = _path + _filter;

	vector<string> return_;

	_finddata_t fd;
	long handle = _findfirst(searching.c_str(), &fd);

	if (handle == -1)    return return_;

	int result = 0;
	do
	{
		cout << fd.name << endl;
		return_.push_back(fd.name);
		result = _findnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);

	return return_;

}

vector<Mat> sharpnessCore::makeImageToMat(vector<string> imageFileNames) {
	int size = imageFileNames.size();
	
	vector<Mat> imageMats(size);

	for (int i = 0; i < imageFileNames.size(); i++) {
		Mat imgMat = imread("C:\\Users\\Administrator\\Desktop\\image\\"+imageFileNames[i], IMREAD_GRAYSCALE);
		

		Mat dst;
		resize(imgMat, dst, Size(400, 300), 0, 0, CV_INTER_LINEAR);

		imageMats[i] = dst;

	}

	return imageMats;
}


vector<int> sharpnessCore::measureSharpnessBySobel(Mat originMat) {
	int rows = originMat.rows;
	int cols = originMat.cols;

	int calcRows = rows / MASTERVALUE;
	int calcCols = cols / MASTERVALUE;

	vector<int> sharpness(MASTERVALUE*MASTERVALUE);

	int key = 0;

	for (int j = 0; j <rows; j += calcRows) {
		for (int i = 0; i < cols; i += calcCols)
		{
			int value = 0;
			Rect rect(i, j, calcCols, calcRows);

			Mat src_gray = originMat(rect);

			Mat grad;

			int scale = 1;
			int delta = 0;
			int ddepth = CV_16S;

			//GaussianBlur(src_gray, src_gray, Size(3, 3), 0, 0, BORDER_DEFAULT);
			//cvtColor(src, src_gray, COLOR_BGR2GRAY);

			Mat grad_x, grad_y;
			Mat abs_grad_x, abs_grad_y;

			Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
			//Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
			Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
			//Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
			convertScaleAbs(grad_x, abs_grad_x);
			convertScaleAbs(grad_y, abs_grad_y);
			addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

			for (int iRows = 0; iRows < grad.rows; iRows++)
			{
				for (int iCols = 0; iCols < grad.cols; iCols++)
				{
					value += (int)grad.at<uchar>(iRows, iCols);
				}
			}
			sharpness[key] = value;
			key++;
		}
	}

	return sharpness;
}

vector<vector<int> > sharpnessCore::measureSharpnessBySobel(vector<Mat> imageMats) {
	int size = imageMats.size();
	vector<vector<int> > sharpnessVector(size);

	for (int iMat = 0; iMat < imageMats.size(); iMat++) {
		Mat originMat = imageMats[iMat].clone();

		int rows = originMat.rows;
		int cols = originMat.cols;

		int calcRows = rows / MASTERVALUE;
		int calcCols = cols / MASTERVALUE;

		vector<int> sharpness(MASTERVALUE*MASTERVALUE);

		int key = 0;

		for (int j = 0; j <rows; j += calcRows) {
			for (int i = 0; i < cols; i += calcCols)
			{
				int value = 0;
				Rect rect(i, j, calcCols, calcRows);

				Mat src_gray = originMat(rect);

				Mat grad;

				int scale = 1;
				int delta = 0;
				int ddepth = CV_16S;

				//GaussianBlur(src_gray, src_gray, Size(3, 3), 0, 0, BORDER_DEFAULT);
				//cvtColor(src, src_gray, COLOR_BGR2GRAY);

				Mat grad_x, grad_y;
				Mat abs_grad_x, abs_grad_y;

				//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
				Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
				//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
				Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
				convertScaleAbs(grad_x, abs_grad_x);
				convertScaleAbs(grad_y, abs_grad_y);
				addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

				for (int iRows = 0; iRows < grad.rows; iRows++)
				{
					for (int iCols = 0; iCols < grad.cols; iCols++)
					{
						value += (int)grad.at<uchar>(iRows, iCols);
					}
				}
				sharpness[key] = value;
				key++;
			}
		}

		sharpnessVector[iMat] = sharpness;
	}

	cout << "sharpnessVector size: "<< sharpnessVector.size() << endl;
	return sharpnessVector;
}

vector<int> sharpnessCore::compareSharpness(vector<int> img1, vector<int> img2) {
	vector<int> sharpnessOrder(MASTERVALUE*MASTERVALUE);

	for (int i = 0; i < MASTERVALUE*MASTERVALUE; i++) {
		if (img1[i] > img2[i]) {
			sharpnessOrder[i] = 1;
			cout << ">" << endl;
		}
		else if (img1[i] == img2[i]) {
			sharpnessOrder[i] = 1;
			cout << "==" << endl;
		}
		else if (img1[i] < img2[i]) {
			sharpnessOrder[i] = 2;
			cout << "<" << endl;
		}
	}

	return sharpnessOrder;
}

vector<int> sharpnessCore::compareSharpness(vector<vector<int> >sharpnessVector) {
	vector<int> sharpnessOrder(MASTERVALUE*MASTERVALUE);
	vector<int> maxValue(MASTERVALUE*MASTERVALUE);
	maxValue.assign(MASTERVALUE*MASTERVALUE, 0);

	for (int i = 0; i < sharpnessVector.size(); i++) {
		for (int j = 0; j < sharpnessVector[i].size(); j++) {
			if (maxValue[j] <= sharpnessVector[i][j]) {
				maxValue[j] = sharpnessVector[i][j];
				sharpnessOrder[j] = i;
			}
		}
	}

	return sharpnessOrder;
}


Mat sharpnessCore::createImage(Mat imgMat1, Mat imgMat2, vector<int> sharpnessOrder) {

	Mat resultMat = imgMat1.clone();

	int rows = imgMat1.rows;
	int cols = imgMat1.cols;

	int calcRows = rows / MASTERVALUE;
	int calcCols = cols / MASTERVALUE;


	int key = 0;
	for (int iRows = 0; iRows < rows; iRows += calcRows)
	{
		for (int iCols = 0; iCols < cols; iCols += calcCols)
		{
			Mat inputMat;

			Rect rect(iCols, iRows, calcCols, calcRows);

			Mat imageROI = resultMat(rect);

			if (sharpnessOrder[key] == 0) {
				inputMat = imgMat1(rect);
			}
			else {
				inputMat = imgMat2(rect);
			}

			inputMat.copyTo(imageROI, inputMat);


			key++;
		}
	}

	return resultMat;

}

Mat sharpnessCore::createImage(vector<Mat> imageMats, vector<int> sharpnessOrder) {

	Mat imgMat1 = imageMats[0];

	Mat resultMat = imgMat1.clone();

	int rows = imgMat1.rows;
	int cols = imgMat1.cols;

	int calcRows = rows / MASTERVALUE;
	int calcCols = cols / MASTERVALUE;

	int key = 0;
	for (int iRows = 0; iRows < rows; iRows += calcRows)
	{
		for (int iCols = 0; iCols < cols; iCols += calcCols)
		{
			Mat inputMat;

			Rect rect(iCols, iRows, calcCols, calcRows);

			Mat imageROI = resultMat(rect);

			for (int i = 0; i < imageMats.size(); i++) {
				if (sharpnessOrder[key] == i) {
					inputMat = imageMats[i](rect);
				}
			}
			
			inputMat.copyTo(imageROI, inputMat);

			//cv::addWeighted(imageROI, 0.3, inputMat, 0.3, 0.0, imageROI);
			// imageROI = 0.0*imageROI + 1.0*inputMat + 0
			key++;
		}
	}

	return resultMat;

}