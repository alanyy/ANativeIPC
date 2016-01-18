#include "TestService.h"
#include "ServiceBaseDefines.h"
#include "Log.h"
#include "utils/AndroidThreads.h"
bool TestService::m_quit = false;
android::sp<android::IBinder> TestService::m_binder = NULL;
android::Condition TestService::m_condition;
android::Mutex TestService::m_lock;
TestService::TestService()
    : ServiceBase("testservice")
{
}

TestService::~TestService()
{
}

int TestService::onRequest(unsigned int code, const android::Parcel &data, android::Parcel *reply, unsigned int flags)
{
    BSLOGD("TestService::onRequest code is %d", code);
    if(code == ADD_ASYNC_BINDER) {
        m_binder = data.readStrongBinder();
        BSLOGD("TestService::onRequest add async binder is %x", m_binder.get());
        androidCreateThread(TestService::asynthread, NULL);
        reply->writeInt32(BS_NO_ERROR);
        return BS_NO_ERROR;
    }
    else if(code == Asyn_Command) {
        const char *str = data.readCString();
        BSLOGD("TestService::onRequest async call string is %s", str);
        m_condition.signal();
        return BS_NO_ERROR;
    }
    else {
        const char *str = data.readCString();
        BSLOGD("TestService::onRequest sync call string is %s", str);
        if(reply) {
            reply->writeCString("I am service");
        }
    }
    return BS_NO_ERROR;
}

int TestService::asynthread(void *arg)
{
    do {
        m_condition.wait(m_lock);
        if(m_quit) {
            break;
        }
        android::Parcel data;
        data.writeCString("asynchronous call reply");
        android::status_t ret = m_binder->transact(Asyn_Command, data, NULL, ASYN_CALL);
        BSLOGD("TestService::asynthread transact ret is %d", ret);
    }
    while(true);
    return 0;
}
