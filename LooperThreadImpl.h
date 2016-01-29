#ifndef LOOPERTHREADIMPL_H
#define LOOPERTHREADIMPL_H

#include "utils/Thread.h"
#include "utils/Condition.h"
#include "ServiceBaseGlobal.h"
#include <list>

class Message;
class MessageHandler;

class LooperThreadImpl : public android::Thread
{
public:
    LooperThreadImpl();
    virtual ~LooperThreadImpl();

    int stop();

    int postMessage(const android::sp<Message> &msg, int delayMs);
    void registerHandler(const android::sp<MessageHandler> &msgHandler);
private:
    DISABLE_COPY(LooperThreadImpl)

    static int64_t getNowMs();
    virtual bool threadLoop();

    struct Event {
        int64_t whenMs;
        android::sp<Message> message;
    };

    android::sp<MessageHandler> m_msgHandler;
    android::Mutex m_eventLock;
    android::Condition m_eventListConditon;
    std::list<Event> m_eventList;
};

#endif // LOOPERTHREADIMPL_H
