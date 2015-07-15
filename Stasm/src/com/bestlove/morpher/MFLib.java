package com.bestlove.morpher;

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.imgproc.Imgproc;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.util.Log;

public class MFLib {
	static String TAG = "MFLib";
	static {
	    {
	    	System.loadLibrary("opencv_java3");
	    	Log.e(TAG, " JNI lib loaded opencv successfully");
	    }
	    {
			System.loadLibrary("stasm");
			Log.e(TAG, "JNI lib loaded stasm successfully");                  
	    }
//	    {
//			System.loadLibrary("qhull");
//			Log.e(TAG, "JNI lib loaded qhull successfully");                  
//	    }
//	    {
//			System.loadLibrary("face");
//			Log.e(TAG, "JNI lib loaded face successfully");                  
//	    }
	}

	private native long morpher(long img1, long img2);

	public Bitmap faceMorpher(Bitmap img1, Bitmap img2) {
		Mat mat1 = toGray(img1);
		Log.i(TAG, "img1 : "+img1.getHeight()+" "+img1.getWidth());
		Mat mat2 = toGray(img2);
		Log.i(TAG, "img2 : "+img2.getHeight()+" "+img2.getWidth());
		long resptr = morpher(mat1.getNativeObjAddr(), mat2.getNativeObjAddr());
		Log.i(TAG, "morpher ptr "+resptr);
		Mat res = new Mat(resptr);
		Log.i(TAG, "res : "+res.cols()+" "+res.rows());
		Bitmap bmp = Bitmap.createBitmap(res.cols(), res.rows(), Config.ARGB_8888);
		Utils.matToBitmap(res, bmp);
		return bmp;

	}
	
	public static Mat bitmapToMat(Bitmap bitmap){
		Mat mat=new Mat(bitmap.getHeight(),bitmap.getWidth(),CvType.CV_8SC4);
		Utils.bitmapToMat(bitmap, mat);
		return mat;
	}
	
	public static Mat toGray(Mat mat){
		Mat mat_gray=new Mat();
		Imgproc.cvtColor(mat, mat_gray, Imgproc.COLOR_BGRA2GRAY, 1);
		return mat_gray;
	}

	public static Mat toGray(Bitmap bitmap){
		return toGray(bitmapToMat(bitmap));
	}
	
	private MFLib() {
		// TODO Auto-generated constructor stub
	}

	private static MFLib mfLib = null;

	public static MFLib getInstance() {
		if (mfLib == null) {
			mfLib = new MFLib();
		}
		return mfLib;
	}

}
