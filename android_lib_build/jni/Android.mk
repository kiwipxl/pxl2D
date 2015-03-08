# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

ROOT_CPP_FILES   := $(wildcard $(LOCAL_PATH)/../../pxl2D/src/*.cpp)
ROOT_CPP_FILES   := $(ROOT_CPP_FILES:$(LOCAL_PATH)/%=%)

GRAPHICS_CPP_FILES   := $(wildcard $(LOCAL_PATH)/../../pxl2D/src/graphics/*.cpp)
GRAPHICS_CPP_FILES   := $(GRAPHICS_CPP_FILES:$(LOCAL_PATH)/%=%)

INPUT_CPP_FILES   := $(wildcard $(LOCAL_PATH)/../../pxl2D/src/input/*.cpp)
INPUT_CPP_FILES   := $(INPUT_CPP_FILES:$(LOCAL_PATH)/%=%)

PHYSICS_CPP_FILES   := $(wildcard $(LOCAL_PATH)/../../pxl2D/src/physics/*.cpp)
PHYSICS_CPP_FILES   := $(PHYSICS_CPP_FILES:$(LOCAL_PATH)/%=%)

SYSTEM_CPP_FILES   := $(wildcard $(LOCAL_PATH)/../../pxl2D/src/system/*.cpp)
SYSTEM_CPP_FILES   := $(SYSTEM_CPP_FILES:$(LOCAL_PATH)/%=%)

LOCAL_MODULE    := pxl2D

LOCAL_SRC_FILES := $(ROOT_CPP_FILES)
LOCAL_SRC_FILES += $(GRAPHICS_CPP_FILES)
LOCAL_SRC_FILES += $(INPUT_CPP_FILES)
LOCAL_SRC_FILES += $(PHYSICS_CPP_FILES)
LOCAL_SRC_FILES += $(SYSTEM_CPP_FILES)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../pxl2D/include/

include $(BUILD_SHARED_LIBRARY)
