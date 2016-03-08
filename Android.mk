LOCAL_PATH := $(call my-dir)
#生成binder service的服务端
include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \
    libbinder \
    liblog
LOCAL_MODULE    := libservicebase
LOCAL_SRC_FILES := \
    ServiceBase.cpp \
    ServiceBaseImpl.cpp \
    ServiceBinderProxy.cpp \
    ServiceProxyBase.cpp \
<<<<<<< HEAD
    ServiceProxyBaseImpl.cpp \
    AnonymousBinder.cpp \
    LooperThread.cpp \
    LooperThreadImpl.cpp \
    Message.cpp \
    MessageHandler.cpp

LOCAL_CFLAGS := -DSERVICEBASE_LIBRARY
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)
# Include subdirectory makefiles
# ============================================================

# If we're building with ONE_SHOT_MAKEFILE (mm, mmm), then what the framework
# team really wants is to build the stuff defined by this makefile.
ifeq (,$(ONE_SHOT_MAKEFILE))
include $(call first-makefiles-under,$(LOCAL_PATH))
endif

