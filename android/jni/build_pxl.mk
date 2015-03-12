

LOCAL_PATH := $(call my-dir)


#####################################################################
# prebuilds static freetype2 library
#####################################################################


ifndef APP_CLEAN
$(info ~~~~~~~~~~~Building pxl~~~~~~~~~~~)
else
$(info ~~~~~~~~~~~Cleaning pxl build~~~~~~~~~~~)
endif


include $(CLEAR_VARS)


LOCAL_MODULE := libfreetype
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../pxl2D/lib/freetype/include/
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../pxl2D/lib/freetype/include/freetype2/
LOCAL_SRC_FILES := ../libs/local/armeabi/libfreetype2.a


include $(PREBUILT_STATIC_LIBRARY)


#####################################################################
# prebuilds static libpng library
#####################################################################


include $(CLEAR_VARS)


LOCAL_MODULE := libpng
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../pxl2D/lib/libpng/
LOCAL_SRC_FILES := ../libs/local/armeabi/libpng.a


include $(PREBUILT_STATIC_LIBRARY)


#####################################################################
# builds pxl2D into a static library
#####################################################################


include $(CLEAR_VARS)


ROOT_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../pxl2D/src/*.cpp)
ROOT_SRC_FILES := $(ROOT_SRC_FILES:$(LOCAL_PATH)/%=%)

GRAPHICS_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../pxl2D/src/graphics/*.cpp)
GRAPHICS_SRC_FILES := $(GRAPHICS_SRC_FILES:$(LOCAL_PATH)/%=%)

INPUT_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../pxl2D/src/input/*.cpp)
INPUT_SRC_FILES := $(INPUT_SRC_FILES:$(LOCAL_PATH)/%=%)

PHYSICS_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../pxl2D/src/physics/*.cpp)
PHYSICS_SRC_FILES := $(PHYSICS_SRC_FILES:$(LOCAL_PATH)/%=%)

SYSTEM_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../pxl2D/src/system/*.cpp)
SYSTEM_SRC_FILES := $(SYSTEM_SRC_FILES:$(LOCAL_PATH)/%=%)

SYSTEM_ANDROID_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../pxl2D/src/system/android/*.cpp)
SYSTEM_ANDROID_SRC_FILES := $(SYSTEM_ANDROID_SRC_FILES:$(LOCAL_PATH)/%=%)


LOCAL_MODULE := pxl2D


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../pxl2D/include/


LOCAL_SRC_FILES := $(ROOT_SRC_FILES)
LOCAL_SRC_FILES += $(GRAPHICS_SRC_FILES)
LOCAL_SRC_FILES += $(INPUT_SRC_FILES)
LOCAL_SRC_FILES += $(PHYSICS_SRC_FILES)
LOCAL_SRC_FILES += $(SYSTEM_SRC_FILES)
LOCAL_SRC_FILES += $(SYSTEM_ANDROID_SRC_FILES)


LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -lz


LOCAL_WHOLE_STATIC_LIBRARIES := android_native_app_glue libfreetype libpng


include $(BUILD_STATIC_LIBRARY)


$(call import-module,android/native_app_glue)

