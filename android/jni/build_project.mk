

LOCAL_PATH := $(call my-dir)


#####################################################################
# prebuilds static freetype2 library
#####################################################################


ifndef APP_CLEAN
$(info ~~~~~~~~~~~Building project~~~~~~~~~~~)
else
$(info ~~~~~~~~~~~Cleaning project build~~~~~~~~~~~)
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
# prebuilds static pxl2D library
#####################################################################



include $(CLEAR_VARS)


LOCAL_MODULE := pxl2D
LOCAL_SRC_FILES := ../libs/local/armeabi/libpxl2D.a


include $(PREBUILT_STATIC_LIBRARY)



#####################################################################
# builds pxlExample with the pxl2D lib
#####################################################################


include $(CLEAR_VARS)


LOCAL_MODULE    := native-activity

LOCAL_SRC_FILES := ../../pxlExample/src/Main.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../pxl2D/include/

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -lz

LOCAL_WHOLE_STATIC_LIBRARIES := android_native_app_glue libfreetype libpng pxl2D


include $(BUILD_SHARED_LIBRARY)



$(call import-module,android/native_app_glue)

