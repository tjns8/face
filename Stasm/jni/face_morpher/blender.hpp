/*
 * blender.cpp
 *
 *  Created on: 2015年6月23日
 *      Author: ns
 */

//import cv2
//import numpy as np
//import scipy.sparse

#ifndef MF_BLENDER
#define MF_BLENDER
namespace blender{

cv::Mat mask_from_points(cv::Size size, cv::Point points){
  int radius = 10 ;// # kernel size
  cv::Mat kernel(cv::Size(radius,radius),CV_8U);
  cv::Mat mask(size,CV_8U);
  cv::Point hull;
//  cv::convexHull(std::vector<cv::Point>(points),std::vector<cv::Point>(hull));
  cv::fillConvexPoly(mask,&hull,1,cv::Scalar(255));
  cv::erode(mask,mask,kernel);
  return mask;
}
//""" Apply mask to supplied image
//:param img: max 3 channel image
//:param mask: [0-255] values in mask
//:returns: new image with mask applied
//"""
//cv::Mat apply_mask(cv::Mat img, int mask){		//never be used
//  cv::Mat masked_img=img.clone();
//  float mask255=mask/255;
//  for(int c=0;c<3;c++){
//    masked_img[..., c] = img[..., c] * (mask / 255);
//  }
//  return masked_img;
//}

cv::Mat* weighted_average(cv::Mat* img1, cv::Mat* img2,float  percent=0.5){
  if (percent <= 0){
    return img2;
  }
  if (percent >= 1){
    return img1;
  }
  cv::Mat* mat=new cv::Mat();
  cv::addWeighted(*img1,percent,*img2,1-percent,0,*mat);
  return mat;
}


//int* alpha_feathering(src_img, dest_img, img_mask, blur_radius=15){	//never be used
//  mask = cv2.blur(img_mask, (blur_radius, blur_radius))
//  mask = mask / 255.0
//
//  result_img = np.empty(src_img.shape, np.uint8)
//  for i in xrange(3):
//    result_img[..., i] = src_img[..., i] * mask + dest_img[..., i] * (1-mask)
//
//  return result_img
//}

//int* poisson_blend(cv::Mat img_source,cv::Mat  dest_img, cv::Mat  img_mask,cv::Size offset=(0, 0)){
////  # http://opencv.jp/opencv2-x-samples/poisson-blending
//  img_target = np.copy(dest_img)
////  import pyamg
////  # compute regions to be blended
//  region_source = (
//    max(-offset[0], 0),
//    max(-offset[1], 0),
//    min(img_target.shape[0] - offset[0], img_source.shape[0]),
//    min(img_target.shape[1] - offset[1], img_source.shape[1]))
//  region_target = (
//    max(offset[0], 0),
//    max(offset[1], 0),
//    min(img_target.shape[0], img_source.shape[0] + offset[0]),
//    min(img_target.shape[1], img_source.shape[1] + offset[1]))
//  region_size = (region_source[2] - region_source[0],
//                 region_source[3] - region_source[1])
//
////  # clip and normalize mask image
//  img_mask = img_mask[region_source[0]:region_source[2],
//                      region_source[1]:region_source[3]]
//
////  # create coefficient matrix
//  coff_mat = scipy.sparse.identity(np.prod(region_size), format='lil')
//  for y in range(region_size[0]):
//    for x in range(region_size[1]):
//      if img_mask[y, x]:
//        index = x + y * region_size[1]
//        coff_mat[index, index] = 4
//        if index + 1 < np.prod(region_size):
//          coff_mat[index, index + 1] = -1
//        if index - 1 >= 0:
//          coff_mat[index, index - 1] = -1
//        if index + region_size[1] < np.prod(region_size):
//          coff_mat[index, index + region_size[1]] = -1
//        if index - region_size[1] >= 0:
//          coff_mat[index, index - region_size[1]] = -1
//  coff_mat = coff_mat.tocsr()
//
////  # create poisson matrix for b
//  poisson_mat = pyamg.gallery.poisson(img_mask.shape)
////  # for each layer (ex. RGB)
//  for num_layer in range(img_target.shape[2]):
////    # get subimages
//    t = img_target[region_target[0]:region_target[2],
//                   region_target[1]:region_target[3], num_layer]
//    s = img_source[region_source[0]:region_source[2],
//                   region_source[1]:region_source[3], num_layer]
//    t = t.flatten()
//    s = s.flatten()
//
////    # create b
//    b = poisson_mat * s
//    for y in range(region_size[0]):
//      for x in range(region_size[1]):
//        if not img_mask[y, x]:
//          index = x + y * region_size[1]
//          b[index] = t[index]
//
////    # solve Ax = b
//    x = pyamg.solve(coff_mat, b, verb=False, tol=1e-10)
//
////    # assign x to target image
//    x = np.reshape(x, region_size)
//    x[x > 255] = 255
//    x[x < 0] = 0
//    x = np.array(x, img_target.dtype)
//    img_target[region_target[0]:region_target[2],
//               region_target[1]:region_target[3], num_layer] = x
//
//  return img_target
//}

}

#endif
