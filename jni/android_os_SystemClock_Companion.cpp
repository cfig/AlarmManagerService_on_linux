#include <android_os_SystemClock_Companion.h>
#include <type_traits>
#include <cstdint>

//external: libutils.so, liblog.so
#include <utils/SystemClock.h>
#include <utils/Timers.h>
#include <utils/Log.h>
//external

namespace android {

static_assert(std::is_same<int64_t, jlong>::value, "jlong isn't an int64_t");
static_assert(std::is_same<decltype(uptimeMillis()), int64_t>::value,
        "uptimeMillis signature change, expected int64_t return value");
static_assert(std::is_same<decltype(elapsedRealtime()), int64_t>::value,
        "uptimeMillis signature change, expected int64_t return value");
static_assert(std::is_same<decltype(elapsedRealtimeNano()), int64_t>::value,
        "uptimeMillis signature change, expected int64_t return value");

JNIEXPORT jlong JNICALL Java_android_os_SystemClock_00024Companion_currentThreadTimeMillis(JNIEnv *, jobject) {
    return nanoseconds_to_milliseconds(systemTime(SYSTEM_TIME_THREAD));
}

JNIEXPORT jlong JNICALL Java_android_os_SystemClock_00024Companion_currentThreadTimeMicro(JNIEnv *, jobject) {
    return nanoseconds_to_microseconds(systemTime(SYSTEM_TIME_THREAD));
}

JNIEXPORT jlong JNICALL Java_android_os_SystemClock_00024Companion_currentTimeMicro(JNIEnv *, jobject) {
    struct timeval tv;    
    gettimeofday(&tv, NULL);    
    return tv.tv_sec * 1000000LL + tv.tv_usec;    
}
} //namespace android

/*
 * JNI registration.
 */
static const JNINativeMethod sMethods[] = {
    // All of these are @CriticalNative, so we can defer directly to SystemClock.h for
    // some of these
    { (char*)"uptimeMillis", (char*)"()J", reinterpret_cast<void*>(android::uptimeMillis) },
    { (char*)"elapsedRealtime", (char*)"()J", reinterpret_cast<void*>(android::elapsedRealtime) },
    { (char*)"elapsedRealtimeNanos", (char*)"()J", reinterpret_cast<void*>(android::elapsedRealtimeNano) },

    // SystemClock doesn't have an implementation for these that we can directly call
    { (char*)"currentThreadTimeMillis", (char*)"()J",
        reinterpret_cast<void*>(android::Java_android_os_SystemClock_00024Companion_currentThreadTimeMillis) },
    { (char*)"currentThreadTimeMicro", (char*)"()J",
        reinterpret_cast<void*>(android::Java_android_os_SystemClock_00024Companion_currentTimeMicro) },
    { (char*)"currentTimeMicro", (char*)"()J",
        reinterpret_cast<void*>(android::Java_android_os_SystemClock_00024Companion_currentTimeMicro) },
};

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass c = env->FindClass("android/os/SystemClock$Companion");
    if (c == nullptr) return JNI_ERR;

    // Register your class' native methods.
    int rc = env->RegisterNatives(c, sMethods, sizeof(sMethods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;

    return JNI_VERSION_1_6;
}
