

LOCAL_PATH := $(call my-dir)


#####################################################################
# builds freetype into a static library
#####################################################################


ifndef APP_CLEAN
$(info ~~~~~~~~~~~Building freetype~~~~~~~~~~~)
else
$(info ~~~~~~~~~~~Cleaning freetype~~~~~~~~~~~)
endif


include $(CLEAR_VARS)


FREETYPE_LIB_PATH := ../../pxl2D/lib/freetype


LOCAL_MODULE := freetype2


LOCAL_CFLAGS := -DANDROID_NDK -DFT2_BUILD_LIBRARY=1


LOCAL_C_INCLUDES := $(LOCAL_PATH)/include_all \
		$(LOCAL_PATH)/$(FREETYPE_LIB_PATH)/include/ \
		$(LOCAL_PATH)/$(FREETYPE_LIB_PATH)/src/


LOCAL_SRC_FILES := \
	$(FREETYPE_LIB_PATH)/src/autofit/autofit.c \
	$(FREETYPE_LIB_PATH)/src/base/basepic.c \
	$(FREETYPE_LIB_PATH)/src/base/ftapi.c \
	$(FREETYPE_LIB_PATH)/src/base/ftbase.c \
	$(FREETYPE_LIB_PATH)/src/base/ftbbox.c \
	$(FREETYPE_LIB_PATH)/src/base/ftbitmap.c \
	$(FREETYPE_LIB_PATH)/src/base/ftdbgmem.c \
	$(FREETYPE_LIB_PATH)/src/base/ftdebug.c \
	$(FREETYPE_LIB_PATH)/src/base/ftglyph.c \
	$(FREETYPE_LIB_PATH)/src/base/ftinit.c \
	$(FREETYPE_LIB_PATH)/src/base/ftpic.c \
	$(FREETYPE_LIB_PATH)/src/base/ftstroke.c \
	$(FREETYPE_LIB_PATH)/src/base/ftsynth.c \
	$(FREETYPE_LIB_PATH)/src/base/ftsystem.c \
	$(FREETYPE_LIB_PATH)/src/cff/cff.c \
	$(FREETYPE_LIB_PATH)/src/pshinter/pshinter.c \
	$(FREETYPE_LIB_PATH)/src/psnames/psnames.c \
	$(FREETYPE_LIB_PATH)/src/raster/raster.c \
	$(FREETYPE_LIB_PATH)/src/sfnt/sfnt.c \
	$(FREETYPE_LIB_PATH)/src/smooth/smooth.c \
	$(FREETYPE_LIB_PATH)/src/truetype/truetype.c


include $(BUILD_STATIC_LIBRARY)

include $(call all-subdir-makefiles)

