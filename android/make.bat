call android update project --path . --subprojects --target android-21

rem The following variables define whether to build PXL, the example project or both.
rem Set the value to 1 to build and 0 to not build
rem Make sure you've build PXL at least once so libpxl2D.a is in obj/local/armeai

set BUILD_PXL=1
set BUILD_EXAMPLE_PROJECT=1

if %BUILD_PXL% equ 1 call C:\AndroidDevelopment\android-ndk-r10d\ndk-build APP_BUILD_SCRIPT=jni/build_pxl.mk clean
if %BUILD_EXAMPLE_PROJECT% equ 1 call C:\AndroidDevelopment\android-ndk-r10d\ndk-build APP_BUILD_SCRIPT=jni/build_project.mk clean
if %BUILD_PXL% equ 1 call C:\AndroidDevelopment\android-ndk-r10d\ndk-build APP_BUILD_SCRIPT=jni/build_pxl.mk
if %BUILD_EXAMPLE_PROJECT% equ 1 call C:\AndroidDevelopment\android-ndk-r10d\ndk-build APP_BUILD_SCRIPT=jni/build_project.mk

call ant debug
call adb install -r bin/NativeActivity-debug.apk
call adb shell am start -n com.example.native_activity/android.app.NativeActivity
call adb -d logcat native-activity:w *:s
pause