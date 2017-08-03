#ifndef VORONOIFIELD_HPP
#define VORONOIFIELD_HPP

#include "opencv2/opencv.hpp"
#include <vector>
#include <omp.h>


class Voronoi{
	double CalcDistance(cv::Point a, cv::Point b){
		return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}
	void CellProcessing(cv::Point center, cv::Point side){
		if ((side.x < 0) || (image_size.x <= side.x) || (side.y < 0) || (image_size.y <= side.y)){ return; }

		if (cells[side.x][side.y].flag == MOTHER){
			if (cells[center.x][center.y].flag == NONE){
				cells[center.x][center.y].flag = DETECTED;
				cells[center.x][center.y].distance = CalcDistance(center, side);
				cells[center.x][center.y].point_mother = side;
			}
			else if (cells[center.x][center.y].flag == DETECTED){
				if (cells[center.x][center.y].distance > CalcDistance(center, side)){
					cells[center.x][center.y].distance = CalcDistance(center, side);
					cells[center.x][center.y].point_mother = side;
				}
			}
		}
		else if (cells[side.x][side.y].flag == DETECTED){
			if (cells[center.x][center.y].flag == NONE){
				cells[center.x][center.y].flag = DETECTED;
				cells[center.x][center.y].distance = CalcDistance(center, cells[side.x][side.y].point_mother);
				cells[center.x][center.y].point_mother = cells[side.x][side.y].point_mother;
			}
			else if (cells[center.x][center.y].flag == DETECTED){
				if (cells[center.x][center.y].distance > CalcDistance(center, cells[side.x][side.y].point_mother)){
					cells[center.x][center.y].distance = CalcDistance(center, cells[side.x][side.y].point_mother);
					cells[center.x][center.y].point_mother = cells[side.x][side.y].point_mother;
				}
			}
		}
	}
public:
	class cell_info{
	public:
		int flag;
		cv::Point point_mother;
		double distance;
		cell_info(){
			flag = 0;
			point_mother = cv::Point(-1, -1);
			distance = 0;
		}

	};
	cv::Point image_size;
	double max_distance;
	std::vector<std::vector<cell_info> > cells;

	int NONE;
	int MOTHER;
	int DETECTED;

	Voronoi(){
		NONE = 0;
		MOTHER = 1;
		DETECTED = -1;
	}
	Voronoi(cv::Point size){
		NONE = 0;
		MOTHER = 1;
		DETECTED = -1;

		SetMap(size);
	}
	void SetMap(cv::Point size){
		image_size = size;
		cells = std::vector<std::vector<cell_info> >(image_size.x, std::vector<cell_info>(image_size.y));
		max_distance = CalcDistance(cv::Point(0, 0), size);
	}
	void SetPoints(std::vector<cv::Point> points){
		for (std::vector<cv::Point>::iterator itr = points.begin(); itr < points.end(); ++itr){
			if ((itr->x < 0) || (image_size.x <= itr->x) || (itr->y < 0) || (image_size.y <= itr->y)){ return; }
			cells[itr->x][itr->y].flag = MOTHER;
		}
	}
	void CreateVoronoi(){
		for (int k = image_size.x; k >= 1; k /= 2){
			double naname = sqrt(k*k + k* k);
#pragma omp parallel for
			for (int i = 0; i < image_size.x; ++i){
				for (int j = 0; j < image_size.y; ++j){
					CellProcessing(cv::Point(i, j), cv::Point(i - k, j - k));
					CellProcessing(cv::Point(i, j), cv::Point(i + k, j - k));
					CellProcessing(cv::Point(i, j), cv::Point(i - k, j + k));
					CellProcessing(cv::Point(i, j), cv::Point(i + k, j + k));
					CellProcessing(cv::Point(i, j), cv::Point(i, j - k));
					CellProcessing(cv::Point(i, j), cv::Point(i, j + k));
					CellProcessing(cv::Point(i, j), cv::Point(i - k, j));
					CellProcessing(cv::Point(i, j), cv::Point(i + k, j));
				}
			}
		}
	}
	void GetImage(cv::Mat &image){
#pragma omp parallel for
		for (int i = 0; i < image_size.x; ++i){
			for (int j = 0; j < image_size.y; ++j){
				image.at<unsigned char>(cv::Point(i, j)) = cells[i][j].distance * (512 / max_distance);
			}
		}
	}
	void GetData(cv::Mat &image, double max_d){
#pragma omp parallel for
		for (int i = 0; i < image_size.x; ++i){
			for (int j = 0; j < image_size.y; ++j){
				image.at<double>(cv::Point(i, j)) = cells[i][j].distance / max_d;
			}
		}
	}
};

#endif