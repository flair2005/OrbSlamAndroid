# OrbSlamAndroid

This project is based on FangGet's [ORB_SLAM2_ANDROID](https://github.com/FangGet/ORB_SLAM2_Android)
It is built with Android Studio

## Build
1. Import the project with Gradle
2. After import, Gradle should start building the C++ and C libraries. It won't work.
3. In app/src/main/jni/Android.mk replace OPENCV_MK_PATH with the correct path to the file referenced.
4. Resync with Gradle and it should start building the libraries. It will take time.
