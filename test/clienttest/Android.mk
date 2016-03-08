LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \
    libbinder \
    liblog \
    libservicebase
LOCAL_MODULE    := clienttest
LOCAL_SRC_FILES := \
    main.cpp \
    TestServiceProxy.cpp
LOCAL_C_INCLUDES := frameworks/native/services/ANativeIPC
LOCAL_MODULE_TAGS := optional
include $(BUILD_EXECUTABLE)
