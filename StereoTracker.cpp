#include "StereoTracker.h"
#include <fstream>
#include <ctime>
using namespace cv;
using namespace std;
Mat StereoTracker::Q;
Mat StereoTracker::P1;
Mat StereoTracker::T;
StereoTracker::StereoTracker(void)
{
}


StereoTracker::~StereoTracker(void)
{

}

bool StereoTracker::readCalibFile(const char * file)
{
    FileStorage fs;
	if(fs.open(file,FileStorage::READ))
	{

		//֮ǰ��ȡ�ļ�̫��
		//fs["mapLx"] >> xMapL;
		//fs["mapLy"] >> yMapL;
		//fs["P1"] >> P1;
		//fs["mapRx"] >> xMapR;
		//fs["mapRy"] >> yMapR;
		//fs["P2"] >> P2;
		//fs["Q"] >> Q;
		////calibFlag = 1;
		//fs["T"] >> T;
		//fs["R"] >> R;
		//fs["cameraMatrix_left"] >> cam_matrix_left;
		//fs["cameraMatrix_right"] >> cam_matrix_right;
		//fs["distCoeffs_left"] >> dist_coef_left;
		//fs["distCoeffs_right"] >> dist_coef_right;

		//fs["leftValidArea"] >> leftValidArea;
		//fs["rightValidArea"] >> rightValidArea;

		//new calibfile
		cv::Size ImageSize;
		Mat Rmatlab;


		fs["ImageSize"] >> ImageSize;

		fs["cameraMatrix_left"] >> cam_matrix_left;
		fs["cameraMatrix_right"] >> cam_matrix_right;
		fs["distCoeffs_left"] >> dist_coef_left;
		fs["distCoeffs_right"] >> dist_coef_right;

		fs["R"] >> Rmatlab;
		cv::Rodrigues(Rmatlab,R);
		fs["T"] >> T;



		Mat R1, R2;
		/*  R1�C �����һ�������3x3������� (��ת����) .
		    R2�C ����ڶ��������3x3������� (��ת����) .
		    P1�C�ڵ�һ̨������µ�����ϵͳ(��������)��� 3x4 ��ͶӰ����
		    P2�C�ڵڶ�̨������µ�����ϵͳ(��������)��� 3x4 ��ͶӰ����
			Q�C�������Ӳ�ӳ�����*/
	double alpha = -1.0;          // ˫ĿУ��Ч��������ϵ����ȡֵ 0~1 �� -1
	std::cout<<"calculate..."<<std::endl;
	stereoRectify(
		cam_matrix_left, 
		dist_coef_left, 
		cam_matrix_right, 
		dist_coef_right, 
		ImageSize, 
		R, T, R1, R2, P1, P2, Q, 
		cv::CALIB_ZERO_DISPARITY,
		alpha, 
		ImageSize,
		&leftValidArea, &rightValidArea
		);

	initUndistortRectifyMap(cam_matrix_left, dist_coef_left, R1, P1,ImageSize, CV_32FC1, xMapL, yMapL);
	initUndistortRectifyMap(cam_matrix_right, dist_coef_right, R2, P2, ImageSize, CV_32FC1, xMapR, yMapR);

	return true;

	}
	else
		return false;
}

