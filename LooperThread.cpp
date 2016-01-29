#include "LooperThread.h"
#include "LooperThreadImpl.h"

LooperThread::LooperThread(const android::sp<MessageHandler> &msgHandler)
{
    m_impl = new LooperThreadImpl();
    if(m_impl != NULL) {
        m_impl->registerHandler(msgHandler);
    }
}

LooperThread::~LooperThread()
{
}

int LooperThread::start(const char *name, int priority, unsigned int stack)
{
    if(m_impl != NULL) {
        return m_impl->run(name, priority, stack);
    }
    return BS_NO_MEMORY;
}

int LooperThread::stop()
{
    if(m_impl != NULL) {
        m_impl->requestExit();
        return BS_NO_ERROR;
    }
    return BS_NO_MEMORY;
}

int LooperThread::join()
{
    if(m_impl != NULL) {
        return m_impl->join();
    }
    return BS_NO_MEMORY;
}

int LooperThread::postMessage(const android::sp<Message> &msg, int delayMs)
{
    if(m_impl != NULL) {
        return m_impl->postMessage(msg, delayMs);
    }
    return BS_NO_MEMORY;
}
