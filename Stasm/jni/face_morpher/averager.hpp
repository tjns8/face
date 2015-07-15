/*
 * averager.cpp
 *
 *  Created on: 2015年6月23日
 *      Author: ns
 */



//from docopt import docopt
//import os
//import cv2
//import numpy as np
//import scipy.ndimage
//from matplotlib import pyplot as plt
//import matplotlib.image as mpimg
//import locator
//import aligner
//import warper
//import blender


namespace avarager{

//void list_imgpaths(imgfolder){
//  for fname in os.listdir(imgfolder):
//    if (fname.lower().endswith('.jpg') or
//       fname.lower().endswith('.png') or
//       fname.lower().endswith('.jpeg')):
//      yield os.path.join(imgfolder, fname)
//}
//
//int* sharpen(img){
//  blured = cv2.GaussianBlur(img, (0, 0), 2.5)
//  return cv2.addWeighted(img, 1.4, blured, -0.4, 0)
//}
//
//int* load_image_points(path, size){
//  img = scipy.ndimage.imread(path)[..., :3]
//  points = locator.face_points(path)
//
//  if len(points) == 0:
//    return None, None
//  else:
//    return aligner.resize_align(img, points, size)
//}
//
//void averager(imgpaths, width=500, height=600, alpha=False,
//             blur_edges=False, out_filename='result.png', plot=False){
//  size = (height, width)
//
//  images = []
//  point_set = []
//  for path in imgpaths:
//    img, points = load_image_points(path, size)
//    if img is not None:
//      images.append(img)
//      point_set.append(points)
//
//  ave_points = locator.average_points(point_set)
//  num_images = len(images)
//  result_images = np.zeros(images[0].shape, np.float32)
//  for i in xrange(num_images):
//    print '{0} of {1}'.format(i+1, num_images)
//    result_images += warper.warp_image(images[i], point_set[i],
//                                       ave_points, size, np.float32)
//
//  result_image = np.uint8(result_images / num_images)
//  print 'Processed {0} faces'.format(num_images)
//
//  mask = blender.mask_from_points(size, ave_points)
//  if blur_edges:
//    blur_radius = 10
//    mask = cv2.blur(mask, (blur_radius, blur_radius))
//  if alpha:
//    result_image = np.dstack((result_image, mask))
//  mpimg.imsave(out_filename, result_image)
//
//  if plot:
//    plt.axis('off')
//    plt.imshow(result_image)
//    plt.show()
//
//if __name__ == "__main__":
//  args = docopt(__doc__, version='Face Averager 1.0')
//  averager(list_imgpaths(args['--images']), int(args['--width']),
//           int(args['--height']), args['--alpha'], args['--blur'],
//           args['--out'], args['--plot'])
//
//}

}