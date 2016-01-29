#ifndef LOOPERTHREADIMPL_H
#define LOOPERTHREADIMPL_H

#include "utils/Thread.h"
#include "utils/Condition.h"
#include "ServiceBaseGlobal.h"

class Message;
class MessageHandler;

class LooperThreadImpl : public android::Thread
{
public:
    LooperThreadImpl();
    virtual ~LooperThreadImpl();

    int postMessage(const android::sp<Message> &msg, int delayMs);
    void registerHandler(const android::sp<MessageHandler> &msgHandler);
private:
    DISABLE_COPY(LooperThreadImpl)
    virtual bool threadLoop();
    android::sp<MessageHandler> m_msgHandler;
};

#endif // LOOPERTHREADIMPL_H
