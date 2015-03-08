call android update project --path . --subprojects --target android-21
call C:\AndroidDevelopment\android-ndk-r10d\ndk-build clean
call C:\AndroidDevelopment\android-ndk-r10d\ndk-build
call ant debug
call adb install -r bin/NativeActivity-debug.apk
call adb shell am start -n com.example.native_activity/android.app.NativeActivity
rem this is a comment
call adb -d logcat native-activity:w *:s
pause