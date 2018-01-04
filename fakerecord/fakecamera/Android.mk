LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES := FakeCameraService.cpp \
	../common/IFakeCameraService.cpp

LOCAL_MODULE_TAG := opntional

LOCAL_SHARED_LIBRARIES := liblog libcutils libutils libbinder
LOCAL_MODULE := FakeCameraService
include $(BUILD_EXECUTABLE)
