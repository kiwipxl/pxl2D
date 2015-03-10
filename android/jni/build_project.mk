

LOCAL_PATH := $(call my-dir)


#####################################################################
# prebuilds static freetype2 library
#####################################################################


$(info ~~~~~~~~~~~Prebuilding static freetype2 library~~~~~~~~~~~)


include $(CLEAR_VARS)

LOCAL_MODULE := libfreetype
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../pxl2D/lib/freetype/include/
LOCAL_SRC_FILES := libfreetype2.a


include $(PREBUILT_STATIC_LIBRARY)


#####################################################################
# prebuilds static pxl2D library
#####################################################################


$(info ~~~~~~~~~~~Prebuilding static pxl2D library~~~~~~~~~~~)


include $(CLEAR_VARS)

LOCAL_MODULE := pxl2D
LOCAL_SRC_FILES := ../obj/local/armeabi/libpxl2D.a



include $(PREBUILT_STATIC_LIBRARY)



#####################################################################
# builds pxlExample with the pxl2D lib
#####################################################################

$(info ~~~~~~~~~~~Building shared pxlExample project~~~~~~~~~~~)


include $(CLEAR_VARS)


LOCAL_MODULE    := native-activity

LOCAL_SRC_FILES := ../../pxlExample/src/Main.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../pxl2D/include/

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -lz

LOCAL_WHOLE_STATIC_LIBRARIES := android_native_app_glue libfreetype pxl2D


include $(BUILD_SHARED_LIBRARY)



$(call import-module,android/native_app_glue)

