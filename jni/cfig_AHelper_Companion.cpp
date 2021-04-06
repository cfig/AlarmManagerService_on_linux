#include <cfig_AHelper_Companion.h>
#include <string>
#include <errno.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <android-base/file.h>
#include <android-base/logging.h>

struct BTS {
    uint32_t I2STDMController : 1; 
    uint32_t I2STDMProtocolMode : 1;
    uint32_t BTController : 1;
    uint32_t BTProtocolMode : 1;
    uint32_t Reserved : 28;
};

typedef union _ACP_CONTROLLER_CONFIGURATION_ {
    BTS bitfields;
    BTS bits;
    uint32_t u32All;
} ACP_CONTROLLER_CONFIGURATION;

jstring JNICALL Java_cfig_AHelper_00024Companion_strerror(JNIEnv *env, jobject, jint e) {
    ACP_CONTROLLER_CONFIGURATION cfg;
    cfg.u32All = 0;
    cfg.bits.I2STDMController = 1;
    cfg.bits.I2STDMProtocolMode = 0;
    cfg.bits.BTController = 1;
    int flags = O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC | O_BINARY;
    android::base::unique_fd fd(open("tmp.file", flags, 0644));
    if (fd == -1) {
        LOG(ERROR) << "android::WriteStringToFile open failed";
    } else {
        LOG(INFO) << "android::WriteStringToFile fd opened";
        android::base::WriteFully(fd, (void*)&cfg, sizeof(ACP_CONTROLLER_CONFIGURATION));
    }
    return env->NewStringUTF(strerror(e));
}
