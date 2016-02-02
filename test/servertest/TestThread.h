#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include "LooperThread.h"
#include "MessageHandler.h"

class TestThread : public LooperThread
{
public:
    TestThread();
    ~TestThread();
    void test();
private:
    class TestHandler : public MessageHandler
    {
    public:
        TestHandler();
        ~TestHandler();
        virtual void onReceiveMessage(const android::sp<Message> &msg);
    };
};

#endif // TESTTHREAD_H
