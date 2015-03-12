call android update project --path . --subprojects --target android-21

call C:\AndroidDevelopment\android-ndk-r10d\ndk-build APP_BUILD_SCRIPT=jni/build_freetype.mk NDK_APP_OUT=libs
call C:\AndroidDevelopment\android-ndk-r10d\ndk-build APP_BUILD_SCRIPT=jni/build_libpng.mk NDK_APP_OUT=libs
call C:\AndroidDevelopment\android-ndk-r10d\ndk-build APP_BUILD_SCRIPT=jni/build_pxl.mk NDK_APP_OUT=libs
call C:\AndroidDevelopment\android-ndk-r10d\ndk-build APP_BUILD_SCRIPT=jni/build_project.mk NDK_APP_OUT=libs

rem //install apk
call ant debug
call adb install -r bin/NativeActivity-debug.apk

rem //clear logcat logs
adb -d logcat -c
rem //run app

call adb shell am start -n com.example.native_activity/android.app.NativeActivity

rem //print any info messages to the console
call adb -d logcat pxl:i *:s

pause