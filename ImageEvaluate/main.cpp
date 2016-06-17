
#include <iostream>
#include <string>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

Mat sobel(Mat& src_gray);
double evaluate(Mat& gray, Mat& bin);

int main(int argc, char* argv[])
{
	char input[256] = { 0 };
  char output[256] = { 0 };
	for (int i = 1001; i < 1006; i++)
	{
		sprintf(input, "../test_image/%d.jpg", i);
		std::cout << input << std::endl;

		Mat src_gray = imread(input, 0);
    if (src_gray.empty()) {
      std::cout << "load image error !" << std::endl;
      break;
    }
		Mat grad = sobel(src_gray);

		Mat bin;
		threshold(grad, bin, 30, 255, THRESH_BINARY + THRESH_OTSU);

		double score = evaluate(grad, bin);

		sprintf(output, "%d_%d.jpg", 100000 + int(score), i);

		imwrite((char*)(output+1), src_gray);

	}

	return 0;
}


Mat sobel(Mat& src_gray)
{
	Mat grad;
	/////////////////////////// Sobe l////////////////////////////////////
	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	/// Gradient X
	int ddepth = CV_16S;
	double scale = 1.0;
	double delta = 0.0;
	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
  return grad;
}

double evaluate(Mat& gray, Mat& bin)
{
	int cnt_edge = 0;
	double sum_edge = 0.0;

	for (int y = 0; y < bin.rows; y++)
	{
		for (int x = 0; x < bin.cols; x++)
		{
			if (bin.data[bin.step[0] * y + x] == 255)
			{
				cnt_edge++;
				int value = gray.data[gray.step[0] * y + x];
				sum_edge += value * value;
			}
		}
	}

	//imshow("tmp", tmp);

	return sum_edge / cnt_edge;
}
