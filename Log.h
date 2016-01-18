#ifndef LOG_H
#define LOG_H

#include "cutils/log.h"
#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "servicebase"

#define BSLOGI(...) ALOGI(__VA_ARGS__)
#define BSLOGD(...) ALOGD(__VA_ARGS__)
#define BSLOGW(...) ALOGW(__VA_ARGS__)
#define BSLOGE(...) ALOGE(__VA_ARGS__)

#endif // LOG_H
