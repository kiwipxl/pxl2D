call android update project --path . --subprojects --target android-21

call C:\AndroidDevelopment\android-ndk-r10d\ndk-build APP_BUILD_SCRIPT=jni/build_freetype.mk NDK_APP_OUT=libs
call C:\AndroidDevelopment\android-ndk-r10d\ndk-build APP_BUILD_SCRIPT=jni/build_libpng.mk NDK_APP_OUT=libs
call C:\AndroidDevelopment\android-ndk-r10d\ndk-build APP_BUILD_SCRIPT=jni/build_pxl.mk NDK_APP_OUT=libs
call C:\AndroidDevelopment\android-ndk-r10d\ndk-build APP_BUILD_SCRIPT=jni/build_project.mk NDK_APP_OUT=libs

echo --installing apk--
call ant debug
call adb install -r bin/NativeActivity-debug.apk

echo --clearing logcat--
call adb logcat -c

echo --starting activity--
call adb shell am start -n com.example.native_activity/android.app.NativeActivity

echo --logging pxl messages--
call adb logcat pxl:i *:s

pause