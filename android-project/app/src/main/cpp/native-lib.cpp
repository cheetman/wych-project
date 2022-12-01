#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_tes_project_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_tes_project_MainActivity_sendString(
        JNIEnv* env,
        jobject /* this */,jstring string) {



    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}