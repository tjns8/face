/*
 * warper.cpp
 *
 *  Created on: 2015年6月23日
 *      Author: ns
 */

//import numpy as np
//import scipy.spatial as spatial
#include "init.hpp"
#include "spatial.hpp"
#ifndef MF_WARPER
#define MF_WARPER
namespace warper {

typedef int color;
//using namespace spatial;
//
//
//coords 2x2
//img NxN
//return color[2]
color bilinear_interpolate(cv::Mat* img, float* coords) {
	int x0 = coords[0];
	int y0 = coords[1];
	float dx = coords[0] - x0;
	float dy = coords[1] - y0;

//  # 4 Neighour pixels
	color q11 = img->at<color>(y0, x0);
	color q21 = img->at<color>(y0, x0 + 1);
	color q12 = img->at<color>(y0 + 1, x0);
	color q22 = img->at<color>(y0 + 1, x0 + 1);
	color btm = q21 * dx + q11 * (1 - dx);
	color top = q22 * dx + q12 * (1 - dx);
	color inter_pixel = top * dy + btm * (1 - dy);
	return inter_pixel;
}
//

int** grid_coordinates(float* points) {
	int xmin = INT_MAX, xmax = INT_MIN, ymin = INT_MAX, ymax = INT_MIN;
	for (int i = 0; i < stasm_NLANDMARKS; i++) {
		float x = points[i * 2];
		float y = points[i * 2 + 1];
		if (x < xmin)
			xmin = x;
		if (x > xmax)
			xmax = x;
		if (y < ymin)
			ymin = y;
		if (y > ymax)
			ymax = y;
	}
	xmax++;
	ymax++;
	//这里构造了一组坐标集
	int** coords = new int*[(xmax - xmin) * (ymax - ymin)];
	int ylen = ymax - ymin;
	int xlen = xmax - xmin;
	for (int i = 0; i < ylen; i++) {
		for (int j = 0; j < xlen; j++) {
			int index = i * xlen + j;
			coords[index] = new int[3];
			coords[index][0] = j + xmin;
			coords[index][1] = i + ymin;
			coords[index][2] = 1;
		}
	}
	return coords;
//  return np.asarray([(x, y) for y in xrange(ymin, ymax)
//                     for x in xrange(xmin, xmax)], np.uint32)
}
//返回一组点组
int** getIndexCoords(int** roi_coords, int* roi_tri_indices,
		int roi_tri_indices_len, int simple_index, int* res_sum) {
	int sum = 0;
	for (int i = 0; i < roi_tri_indices_len; i++) {
		if (roi_tri_indices[i] == simple_index)
			sum++;
	}
	int** res = new int*[sum];
	*res_sum = sum;
	sum = 0;
	for (int i = 0; i < roi_tri_indices_len; i++) {
		if (roi_tri_indices[i] == simple_index) {
			res[sum] = roi_coords[i];
			sum++;
		}
	}
	return res;
}
//这里对三角点应用变换矩阵
float* getOutCoord(cv::Mat affines, int* coord) {
	float mat[3][3] = { { affines.at<float>(0, 0), affines.at<float>(0, 1),
			affines.at<float>(0, 2) }, { affines.at<float>(1, 0), affines.at<
			float>(1, 1), affines.at<float>(1, 2) }, { affines.at<float>(2, 0),
			affines.at<float>(2, 1), affines.at<float>(2, 2) } };
	float* outcoord = new float[3];
	outcoord[0] = mat[0][0] * coord[0] + mat[0][1] * coord[1]
			+ mat[0][2] * coord[2];
	outcoord[1] = mat[1][0] * coord[0] + mat[1][1] * coord[1]
			+ mat[1][2] * coord[2];
	outcoord[2] = mat[2][0] * coord[0] + mat[2][1] * coord[1]
			+ mat[2][2] * coord[2];
	return outcoord;
}
//
int process_warp(cv::Mat*src_img, cv::Mat*result_img, cv::Mat* tri_affines,
		float* dst_points, spatial::Delaunay delaunay) {
	int** roi_coords = grid_coordinates(dst_points); //获取点对应的最小矩形蒙版
//  # indices to vertices. -1 if pixel is not in any triangle
	int * roi_tri_indices = delaunay.find_simplex(roi_coords,len(roi_coords));
	//对每个点判断其所属三角形的序号
	int roi_tri_indices_len;
	int simpliceslen = delaunay.simpliceslen;
	for (int simplex_index = 0; simplex_index < simpliceslen; simplex_index++) {
		int coordslen;
		//获取第simplex_index号三角网格所包含的点
		int** coords = getIndexCoords(roi_coords, roi_tri_indices,
				roi_tri_indices_len, simplex_index, &coordslen);
//				roi_coords[roi_tri_indices == simplex_index]; //coords为点坐标
		for (int i = 0; i < coordslen; i++) {
			int* coord = coords[i];
			float* outcoord = getOutCoord(tri_affines[simplex_index], coord);//得到变换后的坐标点
			int x = coord[0];
			int y = coord[1];
			result_img->at<color>(y, x) = bilinear_interpolate(src_img,		//逐个像素进行赋值
					outcoord);
		}
	}
	return 0;
}

float getPointsX(float* points, int index) {
	return points[index * 2];
}
float getPointsY(float* points, int index) {
	return points[index * 2 + 1];
}
//遍历所有三角网格点，生成变化矩阵
cv::Mat* triangular_affine_matrices(int (* vertices)[3], int verticeslen,
		float* src_points, float* dest_points) {
//  ones = [1, 1, 1];
	//src_points[tri_indces,:].T是
	cv::Mat* mats = new cv::Mat[verticeslen];
	for (int i = 0; i < verticeslen; i++) {
		int* tri_indices = vertices[i]; //3单元向量
		//src_tri 		sv0x,	sv1x,	sv2x
		//				sv0y,	sv1y,	sv2y
		//				1		1		1
		float src_tri[3][3] = {
				{ getPointsX(src_points, tri_indices[0]),getPointsX(src_points, tri_indices[1]), getPointsX(src_points,	tri_indices[2]) },
				{ getPointsY(src_points,tri_indices[0]), getPointsY(src_points, tri_indices[1]),getPointsY(src_points, tri_indices[2]) },
				{ 1, 1, 1 } };
		//dst_tri		dv0x,	dv1x,	dv2x
		//				dv0y,	dv1y,	dv2y
		//				1		1		1
		float dst_tri[3][3] = { { getPointsX(dest_points, tri_indices[0]),
				getPointsX(dest_points, tri_indices[1]), getPointsX(dest_points,
						tri_indices[2]) }, { getPointsY(dest_points,
				tri_indices[0]), getPointsY(dest_points, tri_indices[1]),
				getPointsY(dest_points, tri_indices[2]) }, { 1, 1, 1 } };
		cv::Mat msrc = cv::Mat(3, 3, CV_32F, src_tri);
		cv::Mat mdst = cv::Mat(3, 3, CV_32F, dst_tri);
		cv::Mat mat = msrc * (mdst.inv());
//    mat = np.dot(src_tri, np.linalg.inv(dst_tri))[:2, :]
//    yield mat
		mats[i] = mat;
		//mat 3x3
	}
	return mats;//matx Nx3x3
}

//src_points x2
//dest_points x2
cv::Mat* warp_image(cv::Mat* src_img, float* src_points, float* dest_points,
		cv::Size dest_shape) {
	int rows = dest_shape.height, cols = dest_shape.width;
//  result_img = np.zeros((rows, cols, num_chans), dtype)
	cv::Mat* result_img = new cv::Mat(dest_shape, CV_8U);
	src_img->copyTo(*result_img);
	spatial::Delaunay delaunay = spatial::Delaunay(dest_points,len(dest_points)); //!!!!!!这里还没有实现
	int(* simplices)[3] = delaunay.simplices; //二维数组 Nx3
	int simplicesLen = delaunay.simpliceslen; //三角数量
	aprintf("create delaunay success");
	cv::Mat* tri_affines = triangular_affine_matrices(simplices, simplicesLen,
			src_points, dest_points);
	process_warp(src_img, result_img, tri_affines, dest_points, delaunay);
	aprintf("warp img success");
	return result_img;
// return src_img;
}

//void test_local(){
////  from functools import partial
////  import scipy.ndimage
////  import scipy.misc
////  import locator
////  import aligner
////  from matplotlib import pyplot as plt
//
//  # Load source image
//  face_points_func = partial(locator.face_points, '../data')
//  base_path = '../females/Screenshot 2015-03-04 17.11.12.png'
//  src_path = '../females/BlDmB5QCYAAY8iw.jpg'
//  src_img = scipy.ndimage.imread(src_path)[:, :, :3]
//
//  # Define control points for warps
//  src_points = face_points_func(src_path)
//  base_img = scipy.ndimage.imread(base_path)[:, :, :3]
//  base_points = face_points_func(base_path)
//
//  size = (600, 500)
//  src_img, src_points = aligner.resize_align(src_img, src_points, size)
//  base_img, base_points = aligner.resize_align(base_img, base_points, size)
//  result_points = locator.weighted_average_points(src_points, base_points, 0.2)
//
//  # Perform transform
//  dst_img1 = warp_image(src_img, src_points, result_points, size)
//  dst_img2 = warp_image(base_img, base_points, result_points, size)
//
//  print 'blending'
//  import blender
//  ave = blender.weighted_average(dst_img1, dst_img2, 0.6)
//  mask = blender.mask_from_points(size, result_points)
//  blended_img = blender.poisson_blend(dst_img1, dst_img2, mask)
//
//  plt.subplot(2, 2, 1)
//  plt.imshow(ave)
//  plt.subplot(2, 2, 2)
//  plt.imshow(dst_img1)
//  plt.subplot(2, 2, 3)
//  plt.imshow(dst_img2)
//  plt.subplot(2, 2, 4)
//
//  plt.imshow(blended_img)
//  plt.show()
//
//if __name__ == "__main__":
//  test_local()
//
//}

}

#endif
