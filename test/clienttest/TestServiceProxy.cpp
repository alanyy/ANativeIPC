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
{
    setupAsyncRequest();
}

TestServiceProxy::~TestServiceProxy()
{
    teardownAsyncRequest();
}

void TestServiceProxy::sendTestCommand()
{
    do {
        android::Parcel data;
        data.writeCString("I am client");
        android::Parcel reply;
        if(!isConnected()) {
            BSLOGD("TestServiceProxy::sendTestCommand not connected!");
            bool connected = tryConnect();
            BSLOGD("TestServiceProxy::sendTestCommand connect is %d", connected);
        }
        int ret = sendSyncRequest(Sync_Command, data, &reply);
        BSLOGD("TestServiceProxy::sendTestCommand sendSyncRequest return %d", ret);
        if(ret == 0) {
            const char *str = reply.readCString();
            BSLOGD("TestServiceProxy::sendTestCommand sendSyncRequest reply str is %s", str);
        }
        sleep(5);
//        android::Parcel data1;
//        prepareAsyncData(data1);
//        data1.writeCString("hello service");
//        sendAsyncRequest(Async_Command, data1);
//        sleep(5);
    }while(true);
}

int TestServiceProxy::onAsyncResponse(unsigned int code, const android::Parcel &reply)
{
    BSLOGD("TestServiceProxy::onAsyncResponse code is %d", code);
    const char *str = reply.readCString();
    BSLOGD("TestServiceProxy::onAsyncResponse reply is %s", str);
    return BS_NO_ERROR;
}
