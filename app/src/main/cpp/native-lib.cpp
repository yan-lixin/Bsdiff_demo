#include <jni.h>
#include <string>

// extern声明在bspatch.c
extern "C" {
extern int p_main(int argc,const char * argv[]);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_bsdiff_sample_MainActivity_bsPatch(JNIEnv *env, jobject instance, jstring oldApk_,
                                                 jstring patch_, jstring output_) {
    // 将java字符串转为C/C++的字符串，转换为UTF-8格式的char指针
    const char *oldApk = env->GetStringUTFChars(oldApk_, 0);
    const char *patch = env->GetStringUTFChars(patch_, 0);
    const char *output = env->GetStringUTFChars(output_, 0);


    // bspatch, oldfile, newfile, patchfile
    const char *argv[] = {"", oldApk, output, patch};
    p_main(4, argv);

    // 释放指向Unicode格式的char指针
    env->ReleaseStringUTFChars(oldApk_, oldApk);
    env->ReleaseStringUTFChars(patch_, patch);
    env->ReleaseStringUTFChars(output_, output);
}