#ifndef LOOPERTHREAD_H
#define LOOPERTHREAD_H

#include "utils/StrongPointer.h"
#include "ServiceBaseDefines.h"
#include "ServiceBaseGlobal.h"
class LooperThreadImpl;
class MessageHandler;
class Message;

class SERVICEBASE_API LooperThread
{
public:
    LooperThread(const android::sp<MessageHandler> &msgHandler);
    virtual ~LooperThread();

    int start(const char* name = 0,
              int priority = THREAD_PRIORITY_DEFAULT,
              unsigned int stack = 0);
    int stop();
    int join();

    int postMessage(const android::sp<Message> &msg, int delayMs = 0);
private:
    DISABLE_COPY(LooperThread)
    android::sp<LooperThreadImpl> m_impl;
};

#endif // LOOPERTHREAD_H
