#include "TestServiceProxy.h"
#include "Log.h"
#include "binder/Parcel.h"
#include "binder/Binder.h"
#include "AnonymousBinder.h"
#include "binder/ProcessState.h"
#include "binder/IPCThreadState.h"
#include "utils/AndroidThreads.h"

TestServiceProxy::TestServiceProxy()
    : ServiceProxyBase("testservice")
    , m_binder(NULL)
{
}

TestServiceProxy::~TestServiceProxy()
{
}

void TestServiceProxy::sendTestCommand()
{
    do {
        android::Parcel data;
        data.writeCString("client sync call");
        android::Parcel reply;
        if(!isConnected()) {
            BSLOGD("TestServiceProxy::sendTestCommand not connected!");
            bool connected = tryConnect();
            BSLOGD("TestServiceProxy::sendTestCommand connect is %d", connected);
        }
        int ret = sendRequest(First_Command, data, &reply);
        BSLOGD("TestServiceProxy::sendTestCommand send request return %d", ret);
        if(ret == 0) {
            const char *str = reply.readCString();
            BSLOGD("TestServiceProxy::sendTestCommand reply str is %s", str);
        }
        sleep(5);
        android::Parcel data1;
        data1.writeCString("client aync call");
        sendRequest(Asyn_Command, data1, NULL, ASYN_CALL);
        sleep(5);
    }while(true);
}

void TestServiceProxy::setupAsynCall()
{
    m_proc = android::ProcessState::self();
    android::ProcessState::self()->startThreadPool();
    m_binder = new AnonymousBinder();
    android::Parcel data;
    android::Parcel reply;
    data.writeStrongBinder(m_binder);
    sendRequest(ADD_ASYNC_BINDER, data, &reply);
    int ret = reply.readInt32();
    BSLOGD("TestServiceProxy::setupAsynCall reply is %d", ret);
//    androidCreateThread(TestServiceProxy::ipcthread, NULL);
}

int TestServiceProxy::ipcthread(void *arg)
{
    android::IPCThreadState::self()->joinThreadPool();
    return 0;
}

