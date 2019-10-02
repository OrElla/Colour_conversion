#pragma warning(disable:4996)

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define  _AFXDL
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>
#include <iterator>
#include <string>
#include <time.h>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <experimental/filesystem>
#include <opencv2/highgui/highgui.hpp>


//using namespace experimental::filesystem;
using namespace std;
using namespace cv;
namespace fs = std::experimental::filesystem;

void grayscale_image(Mat& image);

void get_file_names(list<string>& names, const string& dir);
bool valid_file(const string& file_name);
void color_image(Mat& image);



int main()
{
	auto started = std::chrono::high_resolution_clock::now();


	string folder_path;
	string new_path = folder_path + ".\\newPhotos";
	cout << "Please enter photos path\n";
	cin >> folder_path;
	list<string> file_names;

	if (!fs::is_directory(new_path) || !fs::exists(new_path)) { // Check if src folder exists
		fs::create_directory(new_path); // create src folder
	}

	get_file_names(file_names, folder_path);


	cout << " please enter your wanted conversion[1 = grayscale, 2 = color]" << endl;
	int num;
	cin >> num;
	while (num < 1 || num >2) {
		cout << "wrong number,  try again";
		cin >> num;
	}



	for (const auto& file : file_names)
	{

		Mat image = imread(folder_path + '/' + file, CV_LOAD_IMAGE_COLOR);

		if (!image.data)
			cout << "cannot open file\n";
		num == 1 ? grayscale_image(image) : color_image(image);

		string newFilePath = new_path + "/" + file;
		imwrite(newFilePath, image);

	}

	auto done = std::chrono::high_resolution_clock::now();

	std::cout << "Program time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(done - started).count() << " ns\n";
	getchar();
	return 0;
}

//receives stack of strings and folder path, puts all file names into the stack
void get_file_names(list<string>& names, const string& dir)
{

	for (const auto& entry : fs::directory_iterator(dir)) {
		const auto filenameStr = entry.path().filename().string();
		if (!valid_file(filenameStr)) continue;
		names.push_back(filenameStr);
	}
}


/*
	receives image and grayscales it
*/
void grayscale_image(Mat& image)
{

	cv::MatIterator_<cv::Vec3b> it, end;
	for (it = image.begin<cv::Vec3b>(), end = image.end<Vec3b>(); it != end; ++it) {
		uchar& r = (*it)[2];
		uchar& g = (*it)[1];
		uchar& b = (*it)[0];

		double average = (r + g + +b) / 3;


		r = (uchar)average;
		g = (uchar)average;
		b = (uchar)average;
	}
}

//check if given file name is of valid type
bool valid_file(const string& file_name)
{
	//possible file types
	static set<String> valid_format = { "jpg", "png", "bmp", "tga" };
	//split
	string check_file_name = file_name.substr(file_name.find_last_of(".") + 1);

	if (check_file_name.empty() ||
		valid_format.find(check_file_name) == valid_format.end()) //check if file type is valid
	{
		cout << "bad input file\n";
		return false;
	}
	return true;
}



//colors an image by playing with contrast and brightness
void color_image(Mat& image)
{
	double alpha = 2.5; /*< Simple contrast control */
	int beta = 67;       /*< Simple brightness control */
	cout << " please enter contrast [1.0-3.0]\n";
	cin >> alpha;
	cout << " please enter brightness[0-100]\n";

	cin >> beta;
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			for (int c = 0; c < image.channels(); c++)
			{
				image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha * image.at<Vec3b>(y, x)[c] + beta);
			}
		}
	}
}