LOCAL_PATH := $(call my-dir)
OPENCVROOT:=${LOCAL_PATH}/../../opencv-android-sdk

include $(CLEAR_VARS)

# OpenCV
OPENCV_INSTALL_MODULES:=on
OPENCV_CAMERA_MODULES:=on
OPENCV_LIB_TYPE:=SHARED

include ${OPENCVROOT}/native/jni/OpenCV.mk

LOCAL_MODULE:= stasm 
LOCAL_FILE_LIST = \
				  $(wildcard $(LOCAL_PATH)/stasm/*.cpp) \
				  $(wildcard $(LOCAL_PATH)/stasm/*.h) \
				  $(wildcard $(LOCAL_PATH)/stasm/MOD_1/*.cpp) \
				  $(wildcard $(LOCAL_PATH)/stasm/MOD_1/*.h) \
				  $(wildcard $(LOCAL_PATH)/face_morpher/qhull/*.c) \
				  $(wildcard $(LOCAL_PATH)/face_morpher/qhull/*.h) \
				  $(wildcard $(LOCAL_PATH)/face_morpher/*.cpp) \
				  $(wildcard $(LOCAL_PATH)/face_morpher/*.hpp) \
				  $(wildcard $(LOCAL_PATH)/*.cpp)

LOCAL_SRC_FILES := $(LOCAL_FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_LDLIBS += -ldl -llog -lc
#LOCAL_CFLAGS += -O3 #-fopenmp
#LOCAL_LDFLAGS += -O3 #-fopenmp

include $(BUILD_SHARED_LIBRARY)