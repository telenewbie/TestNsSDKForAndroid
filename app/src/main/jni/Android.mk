MY_DIR_PATH := $(call my-dir)
MCOM_PATH := $(MY_DIR_PATH)
LOCAL_PATH := $(MCOM_PATH)

APP_LIBS_PATH := $(LOCAL_PATH)/../libs
APP_SRC_PATH := $(LOCAL_PATH)/../cpp

include $(CLEAR_VARS)
LOCAL_MODULE := libelevoc_single_rn
LOCAL_MODULE_FILENAME := libelevoc_single_rn
LOCAL_SRC_FILES := $(APP_LIBS_PATH)/${TARGET_ARCH_ABI}/libelevoc_single_rn.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libelevoc_test
LOCAL_MODULE_FILENAME := libelevoc_test
LOCAL_CFLAGS := -D_ANDROID
LOCAL_LDLIBS := -llog -lm -lz

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
# 采用NEON优化技术
LOCAL_ARM_NEON := true
endif

#LOCAL_CFLAGS += -fvisibility=hidden -fvisibility-inlines-hidden
LOCAL_CFLAGS += -pie -fPIE -fPIC -DPLATFORM_ANDROID -mfloat-abi=softfp
LOCAL_LDFLAGS := -fPIE -fPIC -Wl,--no-warn-mismatch
LOCAL_CPPFLAGS:=-std=c++11 -pthread
LOCAL_DISABLE_FORMAT_STRING_CHECKS := true
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true

LOCAL_SHARED_LIBRARIES := libelevoc_single_rn

INC_DIRS = \
    -I$(APP_LIBS_PATH)/${TARGET_ARCH_ABI}/include\
	-I$(APP_SRC_PATH)\

LOCAL_CFLAGS += $(INC_DIRS)


LOCAL_SRC_FILES := \
    $(APP_SRC_PATH)/com_elevoc_voicefrontandroid.cpp\
    $(APP_SRC_PATH)/ElevocTestCenter.cpp\
    $(APP_SRC_PATH)/ElevocLog.c


include $(BUILD_SHARED_LIBRARY)
