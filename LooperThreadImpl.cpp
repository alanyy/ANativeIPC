#include "LooperThreadImpl.h"
#include "Message.h"
#include "MessageHandler.h"

LooperThreadImpl::LooperThreadImpl()
    : m_msgHandler()
{
}

LooperThreadImpl::~LooperThreadImpl()
{
}

int LooperThreadImpl::postMessage(const android::sp<Message> &msg, int delayMs)
{
    return 0;
}

void LooperThreadImpl::registerHandler(const android::sp<MessageHandler> &msgHandler)
{
    m_msgHandler = msgHandler;
}

bool LooperThreadImpl::threadLoop()
{
    return true;
}
