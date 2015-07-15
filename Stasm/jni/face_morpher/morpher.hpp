/*
 * morpher.cpp
 *
 *  Created on: 2015年6月23日
 *      Author: ns
 */
#include "init.hpp"
#include "stasm/stasm.h"
#include "locator.hpp"
#include "aligner.hpp"
#include "warper.hpp"
#include "blender.hpp"

#ifndef MF_MORPHER
#define MF_MORPHER
namespace morpher{
//from docopt import docopt
//import scipy.ndimage
//import numpy as np
//import os
//import locator
//import aligner
//import warper
//import blender
//import plotter
//import videoer

//void verify_args(args){
//	//启动程序，无需翻译
//  if args['--images'] is None:
//    valid = os.path.isfile(args['--src']) & os.path.isfile(args['--dest'])
//    if not valid:
//      print('--src=%s or --dest=%s are not valid images' % (
//        args['--src'], args['--dest']))
//      exit(1)
//  else:
//    valid = os.path.isdir(args['--images'])
//    if not valid:
//      print('--images=%s is not a valid directory' % args['--images'])
//      exit(1)
//}


bool load_image_points(cv::Mat* img, cv::Size size,
		float *landmarks) {
//	cv::Mat img = cv::imread(path);
	aprintf("morpher load_image_points landmarks : %i ptr : %i",len(landmarks),landmarks);
	if (locator::face_points(img,landmarks)) {
		aprintf("No face in load_image_points");
//		*dest_img = 0;
//		*dest_points = 0;
		return false;
	}else{
		aprintf("face in load_image_points");
//		aprintf("load image points : %i ptr : %i",len(points),points);
	}
//	aligner::resize_align(img, points, size, dest_img, dest_points);

//	*dest_points=points;

	return true;
}

//void load_valid_image_points(const char** imgpaths,cv::Size size) {
//for path in imgpaths:
//load_image_points(path, size,&img,&points);
//if img is not None:
//print path
//yield (img, points)
//}

//void list_imgpaths(images_folder = None, src_image = None, dest_image = None) {
//if images_folder is None:
//yield src_image
//yield dest_image
//else:
//for fname in os.listdir(images_folder):
//if (fname.lower().endswith('.jpg') or;
//				fname.lower().endswith('.png') or;
//						fname.lower().endswith('.jpeg')):;
//						yield os.path.join(images_folder, fname);
//					}

//int* alpha_image(img, points) {
//	mask = blender.mask_from_points(img.shape[:2], points);
//	return np.dstack((img, mask));
//}

//void morph(
//		src_img, src_points, dest_img, dest_points, video, width = 500,
//		height = 600
////		,		num_frames = 20, fps = 10, out_frames = None, out_video =
////				None, alpha = False, plot = False
//				) {
//	size = (height, width)
//	stall_frames = np.clip(int(fps * 0.15), 1, fps)
////# Show first & last longer
//	plt = plotter.Plotter(plot, num_images = num_frames, folder = out_frames)
//	num_frames -= (stall_frames * 2)
////# No need to process src and dest image
//
//	plt.plot_one(src_img)
//	video.write(src_img, stall_frames)
//
////# Produce morph frames!
//for percent in np.linspace(1, 0, num=num_frames):
//points = locator.weighted_average_points(src_points, dest_points, percent)
//src_face = warper.warp_image(src_img, src_points, points, size)
//end_face = warper.warp_image(dest_img, dest_points, points, size)
//average_face = blender.weighted_average(src_face, end_face, percent)
//average_face = alpha_image(average_face, points) if alpha else average_face
//plt.plot_one(average_face, 'save')
//		video.write(average_face)
//
//		plt.plot_one(dest_img)
//		video.write(dest_img, stall_frames)
//
//		plt.show()
//	}

	cv::Mat* morpher(cv::Mat* img1,cv::Mat* img2, int width = 500,int  height = 600,float percent=0.5
			) {
		cv::Size size(width,height);
		float src_point[2*stasm_NLANDMARKS]={0};
		float dst_point[2*stasm_NLANDMARKS]={0};
		farrayprintf(src_point);
		if(!load_image_points(img1,size,src_point)){
			aprintf("img1 load face fail");
		}else{
			aprintf("img1 face success : %i ptr : %i",len(src_point),src_point);
		}
		farrayprintf(src_point);
		farrayprintf(dst_point);
		if(!load_image_points(img2,size,dst_point)){
			aprintf("img2 load face fail");
		}else{
			aprintf("img2 face success : %i ptr : %i",len(dst_point),dst_point);
		}
		farrayprintf(dst_point);
		aprintf("points load success");

		float pointsbuf[2*stasm_NLANDMARKS]={0};
		farrayprintf(pointsbuf);
		float* points=locator::weighted_average_points(src_point,dst_point,pointsbuf,percent);
		farrayprintf(pointsbuf);
		if(points==0){
			aprintf("average points fail");
		}else{
			aprintf("average points success %i",len(points));
		}
		cv::Mat* src_face=warper::warp_image(img1,src_point,points,size);
		matprintf(src_face);
		if(!src_face->cols||!src_face->rows){
			aprintf("src face not find");
		}
		cv::Mat* end_face=warper::warp_image(img2,dst_point,points,size);
		matprintf(end_face);
		if(!end_face->cols||!end_face->rows){
			aprintf("end face not find");
		}
		aprintf("warp imgs success");

		cv::Mat* average_face=blender::weighted_average(src_face,end_face,percent);
		matprintf(average_face);
		if(!average_face->cols||!average_face->rows){
			aprintf("ave face fail");
		}
		return average_face;
	}

}

#endif
