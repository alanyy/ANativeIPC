#include "TestService.h"
#include "ServiceBaseDefines.h"
#include "Log.h"
#include "utils/AndroidThreads.h"
bool TestService::m_quit = false;
SenderId TestService::m_sender = -1;
android::Condition TestService::m_condition;
android::Mutex TestService::m_lock;
TestService::TestService()
    : ServiceBase("testservice")
{
    androidCreateThread(TestService::asyncthread, this);
}

TestService::~TestService()
{
    quitAsyncthread();
}

int TestService::onSyncRequest(unsigned int code, const android::Parcel &data, android::Parcel *reply)
{
    BSLOGD("TestService::onSyncRequest code is %d", code);
    if(code == Sync_Command) {
        const char *str = data.readCString();
        BSLOGD("TestService::onSyncRequest string is %s", str);
        if(reply) {
            reply->writeCString("I am service");
        }
        return BS_NO_ERROR;
    }
    return BS_UNKNOWN_TRANSACTION;
}

int TestService::onAsyncRequest(SenderId &id, unsigned int code, const android::Parcel &data)
{
    BSLOGD("TestService::onAsyncRequest code is %d, id is %d", code, id);
    if(code == Async_Command) {
        m_sender = id;
        const char *str = data.readCString();
        BSLOGD("TestService::onAsyncRequest string is %s", str);
        m_condition.signal();
        return BS_NO_ERROR;
    }
    return BS_UNKNOWN_TRANSACTION;
}

void TestService::quitAsyncthread()
{
    m_quit = true;
    m_condition.signal();
}

int TestService::asyncthread(void *arg)
{
    TestService *service = static_cast<TestService *>(arg);
    do {
        m_condition.wait(m_lock);
        if(m_quit) {
            break;
        }
        android::Parcel data;
        data.writeCString("hello client");
        int ret = service->sendAsyncResponse(m_sender, Async_Command, data);
        BSLOGD("TestService::asynthread sendAsyncResponse ret is %d", ret);
    }
    while(true);
    return 0;
}
