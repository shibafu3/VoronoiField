#ifdef _DEBUG
//Debugモードの場合
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_world300d.lib")            // opencv_core
#else
//Releaseモードの場合
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_world300.lib") 
#endif

#include "voronoifield.hpp"

/*標準入出力ライブラリ*/
#include <iostream>
/*可変長リストライブラリ*/
#include <vector>
/*OpenCVライブラリ*/
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui_c.h"
/*処理時間測定用ライブラリ*/
#include <time.h>

#include <omp.h>

using namespace std;
using namespace cv;


int main() {
	int i = 2;
	Voronoi voronoi(Point(128 * i, 128 * i));

	//vector<Point> points(7);

	//points[0] = Point(32 * i, 32 * i);
	//points[1] = Point(20 * i, 10 * i);
	//points[2] = Point(4 * i, 50 * i);
	//points[3] = Point(50 * i, 33 * 8);
	//points[4] = Point(100 * i, 100 * i);
	//points[5] = Point(100 * i, 10 * i);
	//points[6] = Point(10 * i, 100 * i);


	vector<Point> points;
	for (int x = 100; x < 140; ++x){
		for (int y = 100; y < 130; ++y){
			points.push_back(Point(x, y));
		}
	}

	for (int x = 10; x < 14; ++x){
		for (int y = 10; y < 13; ++y){
			points.push_back(Point(x, y));
		}
	}

	voronoi.SetPoints(points);
	voronoi.CreateVoronoi();

	Mat image = Mat(voronoi.image_size, CV_8UC1, Scalar::all(0));
	voronoi.GetImage(image);
	imshow("voronoi", image);
	waitKey(0);

	return 0;

}







//#ifdef _DEBUG
////Debugモードの場合
//#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_world300d.lib")            // opencv_core
//#else
////Releaseモードの場合
//#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_world300.lib") 
//#endif
//
///*標準入出力ライブラリ*/
//#include <stdio.h>
//#include <stdlib.h>
//#include <iostream>
///*数値計算ライブラリ*/
//#define _USE_MATH_DEFINES
//#include <cmath>
//#include <math.h>
///*可変長リストライブラリ*/
//#include <list>
//#include <vector>
///*OpenCVライブラリ*/
//#include "opencv/cv.h"
//#include "opencv2/highgui/highgui_c.h"
//#include "opencv2/opencv.hpp"
//#include "opencv2/superres/optical_flow.hpp"
///*処理時間測定用ライブラリ*/
//#include <time.h>
//
//#include <omp.h>
//
//using namespace std;
//using namespace cv;
//
//
//class Voronoi{
//
//public:
//	class cell_info{
//	public:
//		int flag;
//		Point point_mother;
//		double distance;
//		cell_info(){
//			flag = 0;
//			point_mother = Point(-1, -1);
//			distance = 0;
//		}
//	};
//	Point image_size;
//	double max_distance;
//	vector<vector<cell_info> > cells;
//
//	int NONE;
//	int MOTHER;
//	int DETECTED;
//
//	Voronoi(Point size){
//		NONE = 0;
//		MOTHER = 1;
//		DETECTED = -1;
//
//		image_size = size;
//		cells = vector<vector<cell_info> >(image_size.x, vector<cell_info>(image_size.y));
//		max_distance = CalcDistance(Point(0, 0), size);
//	}
//	double CalcDistance(Point a, Point b){
//		return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
//	}
//	void CellProcessing(Point center, Point side){
//		if ((side.x < 0) || (image_size.x <= side.x) || (side.y < 0) || (image_size.y <= side.y)){ return; }
//
//		if (cells[side.x][side.y].flag == MOTHER){
//			if (cells[center.x][center.y].flag == NONE){
//				cells[center.x][center.y].flag = DETECTED;
//				cells[center.x][center.y].distance = CalcDistance(center, side);
//				cells[center.x][center.y].point_mother = side;
//			}
//			else if (cells[center.x][center.y].flag == DETECTED){
//				if (cells[center.x][center.y].distance > CalcDistance(center, side)){
//					cells[center.x][center.y].distance = CalcDistance(center, side);
//					cells[center.x][center.y].point_mother = side;
//				}
//			}
//		}
//		else if (cells[side.x][side.y].flag == DETECTED){
//			if (cells[center.x][center.y].flag == NONE){
//				cells[center.x][center.y].flag = DETECTED;
//				cells[center.x][center.y].distance = CalcDistance(center, cells[side.x][side.y].point_mother);
//				cells[center.x][center.y].point_mother = cells[side.x][side.y].point_mother;
//			}
//			else if (cells[center.x][center.y].flag == DETECTED){
//				if (cells[center.x][center.y].distance > CalcDistance(center, cells[side.x][side.y].point_mother)){
//					cells[center.x][center.y].distance = CalcDistance(center, cells[side.x][side.y].point_mother);
//					cells[center.x][center.y].point_mother = cells[side.x][side.y].point_mother;
//				}
//			}
//		}
//	}
//	void SetPoints(vector<Point> points){
//		for (vector<Point>::iterator itr = points.begin(); itr < points.end(); ++itr){
//			if ((itr->x < 0) || (image_size.x <= itr->x) || (itr->y < 0) || (image_size.y <= itr->y)){ return; }
//			cells[itr->x][itr->y].flag = MOTHER;
//		}
//	}
//	void CreateVoronoi(){
//		for (int k = image_size.x; k >= 1; k /= 2){
//			double naname = sqrt(k*k + k* k);
//#pragma omp parallel for
//			for (int i = 0; i < image_size.x; ++i){
//				for (int j = 0; j < image_size.y; ++j){
//					CellProcessing(Point(i, j), Point(i - k, j - k));
//					CellProcessing(Point(i, j), Point(i + k, j - k));
//					CellProcessing(Point(i, j), Point(i - k, j + k));
//					CellProcessing(Point(i, j), Point(i + k, j + k));
//					CellProcessing(Point(i, j), Point(i, j - k));
//					CellProcessing(Point(i, j), Point(i, j + k));
//					CellProcessing(Point(i, j), Point(i - k, j));
//					CellProcessing(Point(i, j), Point(i + k, j));
//				}
//			}
//		}
//	}
//	void GetImage(Mat &image){
//#pragma omp parallel for
//		for (int i = 0; i < image_size.x; ++i){
//			for (int j = 0; j < image_size.y; ++j){
//				image.at<unsigned char>(Point(i, j)) = cells[i][j].distance * (512 / max_distance);
//			}
//		}
//	}
//	void GetData(Mat &image, double max_d){
//#pragma omp parallel for
//		for (int i = 0; i < image_size.x; ++i){
//			for (int j = 0; j < image_size.y; ++j){
//				image.at<double>(Point(i, j)) = cells[i][j].distance / max_d;
//			}
//		}
//	}
//};
//int main() {
//	int i = 2;
//	Voronoi voronoi(Point(128 * i, 128 * i));
//
//	//vector<Point> points(7);
//
//	//points[0] = Point(32 * i, 32 * i);
//	//points[1] = Point(20 * i, 10 * i);
//	//points[2] = Point(4 * i, 50 * i);
//	//points[3] = Point(50 * i, 33 * 8);
//	//points[4] = Point(100 * i, 100 * i);
//	//points[5] = Point(100 * i, 10 * i);
//	//points[6] = Point(10 * i, 100 * i);
//
//
//	vector<Point> points;
//	for (int x = 100; x < 140; ++x){
//		for (int y = 100; y < 130; ++y){
//			points.push_back(Point(x, y));
//		}
//	}
//
//	for (int x = 10; x < 14; ++x){
//		for (int y = 10; y < 13; ++y){
//			points.push_back(Point(x, y));
//		}
//	}
//
//	voronoi.SetPoints(points);
//	voronoi.CreateVoronoi();
//
//	Mat image = Mat(voronoi.image_size, CV_8UC1, Scalar::all(0));
//	voronoi.GetImage(image);
//	imshow("voronoi", image);
//	waitKey(0);
//
//	return 0;
//
//}