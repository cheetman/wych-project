#include <jni.h>
#include <string>
#include "exportfunction.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_tes_lib_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    int a = sum(1,2);

    return env->NewStringUTF(hello.c_str());
}

int sum2(int a, int b) {
    return a + b;

}