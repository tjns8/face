/*
 * aligner.cpp
 *
 *  Created on: 2015年6月23日
 *      Author: ns
 */

//"""
//Align face and image sizes
//"""
//from __future__ import division
//import cv2
//import numpy as np


#ifndef MF_ALIGNER
#define MF_ALIGNER
namespace aligner{

// """ Cap a number to ensure positivity
//
//  :param num: positive or negative number
//  :returns: (overflow, capped_number)
//  """
void positive_cap(int num,int* dsta,int* dstb){
  if (num < 0){
//    return new int[]{0, abs(num)};
	  *dsta=0;
	  *dstb=abs(num);
  }else{
//    return new int[]{num, 0	  };
	  *dsta=num;
	  *dstb=0;
  }
};
//""" Align the rectangle into the center and return the top-left coordinates
//within the new size. If rect is smaller, we add borders.
//
//:param rect: (x, y, w, h) bounding rectangle of the face
//:param size: (width, height) are the desired dimensions
//:param scale: scaling factor of the rectangle to be resized
//:returns: 4 numbers. Top-left coordinates of the aligned ROI.
//  (x, y, border_x, border_y). All values are > 0.
//"""
void roi_coordinates(cv::Rect rect, cv::Size size, float scale,int *dsta,int*dstb,int*dstc,int*dstd){
// int  rectx=rect.x, recty=rect.y, rectw=rect.width,recth=rect.height;
//  int new_height=size.height, new_width =size.width;
//  int mid_x = int((rectx + rectw/2) * scale);
//  int mid_y = int((recty + recth/2) * scale);
//  int roi_x = mid_x - new_width/2;
//  int roi_y = mid_y - new_height/2;
//
//  int border_x ;
//  positive_cap(roi_x,&roi_x,&border_x);
//  int border_y ;
//  positive_cap(roi_y,&roi_y, &border_y );
//  *dsta=roi_x;
//  *dstb=roi_y;
//  *dstc=border_x;
//  *dstd=border_y;
}
//""" Calculate the scaling factor for the current image to be
//    resized to the new dimensions
//
//:param rect: (x, y, w, h) bounding rectangle of the face
//:param size: (width, height) are the desired dimensions
//:returns: floating point scaling factor
//"""
float scaling_factor(cv::Rect rect, cv::Size size){
  int new_height=size.height, new_width =size.width;
  int  rect_h=rect.height, rect_w=rect_w;
  int height_ratio = rect_h / new_height;
  int width_ratio = rect_w / new_width;
  float scale = 1;
  if (height_ratio > width_ratio){
    float new_recth = 0.8 * new_height;
    scale = new_recth / rect_h;
  }else{
    float new_rectw = 0.8 * new_width;
    scale = new_rectw / rect_w;
  }
  return scale;
}
//""" Resize image with the provided scaling factor
//
//:param img: image to be resized
//:param scale: scaling factor for resizing the image
//"""
cv::Mat* resize_image(cv::Mat *img, float scale){
  int  cur_height=img->cols, cur_width =img->rows;
  int new_scaled_height = int(scale * cur_height);
  int new_scaled_width = int(scale * cur_width);
  cv::Mat* dst=new cv::Mat();
  cv::resize(*img,*dst,cv::Size(new_scaled_width, new_scaled_height));
  return dst;
}
//""" Resize image and associated points, align face to the center
//  and crop to the desired size
//
//:param img: image to be resized
//:param points: *m* x 2 array of points
//:param size: (height, width) tuple of new desired size
//"""
void resize_align(cv::Mat*img,float* points, cv::Size size,cv::Mat* dest_crop,float** dest_points){
//  int new_height=size.height, new_width =size.width;
// cv::Rect  rect = cv::boundingRect(std::vector<float>(points));
//  float scale = scaling_factor(rect, size);
//  img = resize_image(img, scale);
//
//  int cur_height=img->cols, cur_width=img->rows;
//  int   roi_x, roi_y, border_x, border_y;
//  roi_coordinates(rect, size, scale,&roi_x, &roi_y, &border_x, &border_y);
//  int roi_h = MIN(new_height-border_y, cur_height-roi_y);
//  int roi_w = MIN(new_width-border_x, cur_width-roi_x);
//  cv::Mat* crop(cv::Size(new_height,new_width),img->type());
//  for(int i=0;i<roi_h;i++){
//	  memcpy(
//			  crop->data+(i+border_y)*crop->step+border_x*crop->elemSize(),
//			  img->data+(i+roi_y)*img->step+roi_x*img->elemSize(),
//			  roi_w*crop->elemSize()
//			  );
//  }
//  int d_x=border_x-roi_x;
//  int d_y=border_y-roi_y;
//  for(int i=0;i<len(points);i+=2){
//	  points[i]=points[i]*scale+d_x;
//	  points[i+1]=points[i+1]*scale+d_y;
//  }
//  *dest_crop=crop;
//  *dest_points=points;
}

}

#endif
