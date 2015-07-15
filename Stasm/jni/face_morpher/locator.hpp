/*
 * locator.cpp
 *
 *  Created on: 2015年6月23日
 *      Author: ns
 */
#include "stasm/stasm.h"
#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "init.hpp"
//"""
//Locate face points
//"""

#ifndef MF_LOCATOR
#define MF_LOCATOR

namespace locator{

//
//import cv2
//import numpy as np
//import subprocess
//import os.path as path
//  """ Produce additional boundary points
//
//  :param points: *m* x 2 array of x,y points
//  :returns: 2 additional points at the top corners
//  """
//std::vector<cv::Point> boundary_points(cv::Point points) {
//	cv::Rect rect = cv::boundingRect(std::vector<cv::Point>(points));
//	int x = rect.x, y = rect.y, w = rect.width, h = rect.height;
//	float buffer_percent = 0.1;
//	int spacerw = int(w * buffer_percent);
//	int spacerh = int(h * buffer_percent);
//	return std::vector<cv::Point>(cv::Point(x + spacerw, y + spacerh),
//			cv::Point(x + w - spacerw, y + spacerh));
//}

//""" Locates 77 face points using stasm (http://www.milbo.users.sonic.net/stasm)
//
//:param imgpath: an image path to extract the 77 face points
//:param add_boundary_points: bool to add 2 additional points
//:returns: Array of x,y face points. Empty array if no face found//"""


bool face_points(cv::Mat* img,float* landmarks, bool add_boundary_points = true) {
	const char* path = "/data/data/org.stasmdemo.android/app_stasm/testface.jpg";
//	cv::Mat img=cv::imread(imgpath,CV_LOAD_IMAGE_GRAYSCALE);//注意！！这里需要完成img转换
	if(!img->data){
		aprintf("-------stasm -------------  img load fail");
		return false;
	}
	int foundface;
	aprintf("locator face_points landmarks : %i ptr : %i",len(landmarks),landmarks);
	if(!stasm_search_single(&foundface,landmarks,(const char*)img->data,img->cols,img->rows,path,"/data/data/org.stasmdemo.android/app_stasm/")){
		aprintf("--------stasm------------error in stasm search single");
		return false;
	}
	aprintf("locator face_points landmarks : %i ptr : %i",len(landmarks),landmarks);
	if(!foundface){
		aprintf("-------stasm-------------no face found");
	}else{
		stasm_force_points_into_image(landmarks,img->cols,img->rows);
	}
	aprintf("locator face_points landmarks : %i ptr : %i",len(landmarks),landmarks);
	return true;
	}

//""" Averages a set of face points from images
//
//:param point_set: *n* x *m* x 2 array of face points. \\
//*n* = number of images. *m* = number of face points per image
//"""
//	std::vector<cv::Point> average_points(std::vector<cv::Point> point_set) {
////		  np.mean(point_set, 0).astype(np.int32)
//		return point_set;
//	}

//""" Weighted average of two sets of supplied points
//
//:param start_points: *m* x 2 array of start face points.
//:param end_points: *m* x 2 array of end face points.
//:param percent: [0, 1] percentage weight on start_points
//:returns: *m* x 2 array of weighted average points
//"""
	float* weighted_average_points(
			float* start_points,
			float* end_points,
			float* points,
			float percent = 0.5) {
		if (percent <= 0) {
			return end_points;
		}
		if (percent >= 1) {
			return start_points;
		}
		for(int i=0;i<2*stasm_NLANDMARKS;i++){
			points[i]=start_points[i]*percent+end_points[i]*(1-percent);
		}
		return points;
}

}

#endif
