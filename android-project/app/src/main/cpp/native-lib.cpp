#include <jni.h>
#include <string>
#include "test.h"
#include "lib.h"

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
        jobject /* this */,jstring string ,jint a) {

   int aaa = sum2(1,3);
    int bb = sum(1,3);
//    int ccc = sum3(1,3);

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}