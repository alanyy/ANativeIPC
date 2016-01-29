#include "LooperThreadImpl.h"
#include "Message.h"
#include "MessageHandler.h"
#include "ServiceBaseDefines.h"
#include "utils/Timers.h"
#include "Log.h"

const long long NANOSECOND_PER_MILLISECOND = 1000000ll;
LooperThreadImpl::LooperThreadImpl()
    : m_msgHandler()
    , m_eventLock()
    , m_eventListConditon()
    , m_eventList()
{
}

LooperThreadImpl::~LooperThreadImpl()
{
    stop();
    join();
}

int LooperThreadImpl::stop()
{
    requestExit();
    m_eventListConditon.signal();
    return BS_NO_ERROR;
}

int LooperThreadImpl::postMessage(const android::sp<Message> &msg, int delayMs)
{
    if(exitPending()) {
        return BS_INVALID_OPERATION;
    }
    android::Mutex::Autolock _l(m_eventLock);
    int64_t whenMs;
    if (delayMs > 0) {
        whenMs = getNowMs() + delayMs;
    } else {
        whenMs = getNowMs();
    }

    std::list<Event>::iterator it = m_eventList.begin();
    for(;it != m_eventList.end(); ++it) {
        if((*it).whenMs > whenMs) {
            break;
        }
    }

    Event event;
    event.message = msg;
    event.whenMs = whenMs;

    if(it == m_eventList.begin()) {
        m_eventListConditon.signal();
    }
    m_eventList.insert(it, event);
    return BS_NO_ERROR;
}

void LooperThreadImpl::registerHandler(const android::sp<MessageHandler> &msgHandler)
{
    android::Mutex::Autolock _l(m_eventLock);
    m_msgHandler = msgHandler;
}

int64_t LooperThreadImpl::getNowMs()
{
    return systemTime(SYSTEM_TIME_MONOTONIC) / NANOSECOND_PER_MILLISECOND;
}

bool LooperThreadImpl::threadLoop()
{
    Event event;
    {
        android::Mutex::Autolock _l(m_eventLock);
        if(exitPending()) {
            return false;
        }
        if(m_eventList.empty()) {
            m_eventListConditon.wait(m_eventLock);
            return true;
        }
        int64_t whenMs = (*(m_eventList.begin())).whenMs;
        int64_t nowMs = getNowMs();

        if (whenMs > nowMs) {
            int64_t delayMs = whenMs - nowMs;
            m_eventListConditon.waitRelative(m_eventLock, delayMs * NANOSECOND_PER_MILLISECOND);

            return true;
        }

        event = *(m_eventList.begin());
        m_eventList.erase(m_eventList.begin());
    }
    if(m_msgHandler != NULL) {
        m_msgHandler->onReceiveMessage(event.message);
    }
    return true;
}
