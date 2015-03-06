call android update project --path . --subprojects --target android-21
call C:\AndroidDevelopment\android-ndk-r10d\ndk-build clean
call C:\AndroidDevelopment\android-ndk-r10d\ndk-build
rem call ant debug
rem call adb install -r bin/NativeActivity-debug.apk
rem call adb shell am start -n com.example.native_activity/android.app.NativeActivity
pause