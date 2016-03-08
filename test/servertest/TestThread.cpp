#include "TestThread.h"
#include "Log.h"
#include "Message.h"
#include "utils/Timers.h"
#include "utils/AndroidThreads.h"

TestThread::TestThread()
    : LooperThread(new TestHandler())
{
    BSLOGD("TestThread::TestThread");
}

TestThread::~TestThread()
{
    BSLOGD("TestThread::~TestThread");
}

void TestThread::test()
{
    do {
        Message *a = new Message(1);
        a->setInt(10);
        a->setPointer(a);
        BSLOGD("[TID:%d][%lld]TestThread::test  post msg %d", (unsigned int)(androidGetThreadId()), systemTime(), a->what());
        postMessage(a);
        sleep(5);
        Message *b = new Message(2);
        b->setString(android::String8("abc"));
        b->setDouble(1.01);
        b->setPointer(b);
        BSLOGD("[TID:%d][%lld]TestThread::test  post msg %d", (unsigned int)(androidGetThreadId()), systemTime(), b->what());
        postMessage(b, 500);
        sleep(5);
    }while(true);
}


TestThread::TestHandler::TestHandler()
{
    BSLOGD("TestHandler::TestHandler");
}

TestThread::TestHandler::~TestHandler()
{
    BSLOGD("TestHandler::~TestHandler");
}

void TestThread::TestHandler::onReceiveMessage(const android::sp<Message> &msg)
{
    int what = msg->what();
    BSLOGD("[TID:%d][%lld]TestHandler::onReceiveMessage msg is %d", (unsigned int)(androidGetThreadId()), systemTime(), what);
    int cnt = msg->valueCount();
    for(int i = 0; i < cnt; ++i) {
        Message::Type type;
        msg->getItemType(i, &type);
        BSLOGD("TestHandler::onReceiveMessage type is %d", type);
        switch(type) {
        case Message::TypeInt:
        {
            int intValue;
            msg->getIntValue(i, &intValue);
            BSLOGD("TestHandler::onReceiveMessage arg %d is %d", i, intValue);
            break;
        }
        case Message::TypeUInt:
        {
            unsigned int uIntValue;
            msg->getUIntValue(i, &uIntValue);
            BSLOGD("TestHandler::onReceiveMessage arg %d is %d", i, uIntValue);
            break;
        }
        case Message::TypeDouble:
        {
            double doubleValue;
            msg->getDoubleValue(i, &doubleValue);
            BSLOGD("TestHandler::onReceiveMessage arg %d is %f", i, doubleValue);
            break;
        }
        case Message::TypeFloat:
        {
            float floatValue;
            msg->getFloatValue(i, &floatValue);
            BSLOGD("TestHandler::onReceiveMessage arg %d is %f", i, floatValue);
            break;
        }
        case Message::TypePointer:
        {
            void *ptr;
            msg->getPointerValue(i, &ptr);
            BSLOGD("TestHandler::onReceiveMessage arg %d is %x", i, (unsigned int)ptr);
            break;
        }
        case Message::TypeString:
        {
            android::String8 str;
            msg->getStringValue(i, &str);
            BSLOGD("TestHandler::onReceiveMessage arg %d is %s", i, str.string());
            break;
        }
        default:
            BSLOGD("invalid arg");
            break;
        }
    }
}
