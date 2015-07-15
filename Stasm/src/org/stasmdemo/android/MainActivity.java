package org.stasmdemo.android;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.imgproc.Imgproc;
import org.opencv.objdetect.CascadeClassifier;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.ImageView;

import com.bestlove.morpher.MFLib;

public class MainActivity extends Activity implements CvCameraViewListener2 ,OnClickListener{
	private static final String TAG ="MainActivity";
	
	private MFLib mfLib=MFLib.getInstance();
	private Mat mRgba;
	private Mat mGray;
	public int[] points;
	
	Bitmap img1,img2;
	
	private File 					mCascadeFile;
	private CameraBridgeViewBase   	mOpenCvCameraView;
    private CascadeClassifier 		mJavaDetector;
	
	private native int[] findFaceLandmarks(long matAddrGr);
	
	
	private void isRawDataExists(Context context){
		try{
			File internalDir = context.getDir("stasm", Context.MODE_PRIVATE);
			File frontalface_xml   = new File(internalDir, "haarcascade_frontalface_alt2.xml");
			//File frontalface_xml = new File(internalDir, "lbpcascade_frontalface.xml");
			File lefteye_xml       = new File(internalDir, "haarcascade_mcs_lefteye.xml");
			File righteye_xml      = new File(internalDir, "haarcascade_mcs_righteye.xml");
			File mounth_xml        = new File(internalDir, "haarcascade_mcs_mounth.xml");
			
			if(frontalface_xml.exists() && lefteye_xml.exists() && righteye_xml.exists() && mounth_xml.exists()){
				Log.d(TAG, "RawDataExists");
			}
			else{
				copyRawDataToInternal(context, R.raw.haarcascade_frontalface_alt2, frontalface_xml);
				copyRawDataToInternal(context, R.raw.haarcascade_mcs_lefteye, lefteye_xml);
				copyRawDataToInternal(context, R.raw.haarcascade_mcs_righteye, righteye_xml);
				copyRawDataToInternal(context, R.raw.haarcascade_mcs_mouth, mounth_xml);
			}
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	private void copyRawDataToInternal(Context context, int id, File file){
		Log.d(TAG, "copyRawDataToInternal: " + file.toString());
		try{
			InputStream is = context.getResources().openRawResource(id);
			FileOutputStream fos = new FileOutputStream(file);
			
			int data;
			byte[] buffer = new byte[4096];
			while((data = is.read(buffer)) != -1){
				fos.write(buffer, 0, data);
			}
			is.close();
			fos.close();
		}catch(IOException e){
			e.printStackTrace();
		}
		Log.d(TAG, "copyRawDataToInternal done");
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.d(TAG, "called onCreate");
		super.onCreate(savedInstanceState);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		setContentView(R.layout.opencv_surface_view);
		
		mOpenCvCameraView = (CameraBridgeViewBase)findViewById(R.id.opencv_java_surface_view);
	    mOpenCvCameraView.setCvCameraViewListener(this);
	    
	    // 2014.06.10 Elvis copy all raw data to internal
		isRawDataExists(MainActivity.this);
		
		findViewById(R.id.button1).setOnClickListener(this);
		//onClick(null);
		
	}
	
	public void onResume() {
		Log.d(TAG, "called onResume");
		super.onResume();
		mOpenCvCameraView.enableView();
	}
	
	@Override
	public void onPause() {
		Log.d(TAG, "called onPause");
		super.onPause();
		if(mOpenCvCameraView != null)
			mOpenCvCameraView.disableView();
	}
	
	public void onDestroy(){
		Log.d(TAG, "called onDestroy");
		super.onDestroy();
		if(mOpenCvCameraView != null)
			mOpenCvCameraView.disableView();
	}

	@Override
	public void onCameraViewStarted(int width, int height) {
		// TODO Auto-generated method stub
		Log.d(TAG, "called onCameraViewStarted");
	    
		mRgba = new Mat(width, height, CvType.CV_8UC3);
		mGray = new Mat(width, height, CvType.CV_8UC1);
	}

	@Override
	public void onCameraViewStopped() {
		// TODO Auto-generated method stub
    	Log.d(TAG, "called onCameraViewStopped");
    	
    	mRgba.release();
		mGray.release();
	}

	@Override
	public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
		// TODO Auto-generated method stub
		Log.d(TAG, "called onCameraFrame");
		
		mRgba = inputFrame.rgba();
		mGray = inputFrame.gray();
		
		// 2014.09.10 Elvis add to flip screen 
		Core.flip(mRgba, mRgba, 1);
		Core.flip(mGray, mGray, 1);
		
		{			
//		mGray=MFLib.bitmapToMat(img1);
//		mGray=MFLib.toGray(mGray);
		}
		
		//2014.09.10 Elvis add to find face feature points  
		points = findFaceLandmarks(mGray.getNativeObjAddr());
		
		//2014.09.10 Elvis add to display face feature points  
		if(points[0] > 0){
			Point pt;
			for(int i=0; i < points.length/2; ++i){
				pt = new Point(points[i*2], points[i*2+1]);
//				Core.circle(mRgba, pt, 4, new Scalar(0, 255, 0));
				Imgproc.circle(mRgba, pt, 4, new Scalar(0, 255, 0));
			}
		}
		
		return mRgba;
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		ImageView imageView=(ImageView) findViewById(R.id.opencv_test_img);
		img1=((BitmapDrawable)getResources().getDrawable(R.drawable.img1)).getBitmap();
		img2=((BitmapDrawable)getResources().getDrawable(R.drawable.img2)).getBitmap();
		Bitmap img3=MFLib.getInstance().faceMorpher(img1, img2);
		imageView.setImageBitmap(img3);
	}
	
}