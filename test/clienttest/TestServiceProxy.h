#ifndef TESTSERVICEPROXY_H
#define TESTSERVICEPROXY_H

#include "ServiceProxyBase.h"
#include "ServiceBaseDefines.h"
#include "utils/Condition.h"

class AnonymousBinder;

class TestServiceProxy : public ServiceProxyBase
{
public:
    enum commandId {
        Sync_Command = SERVICE_FIRST_TRANSACTION,
        Async_Command
    };

    TestServiceProxy();
    ~TestServiceProxy();

    void sendTestCommand();

    virtual int onAsyncResponse(unsigned int code, const android::Parcel &reply);
//    void setupAsynCall();
//    static int ipcthread(void *arg);
private:
//    android::sp<AnonymousBinder> m_binder;
//    android::sp<android::ProcessState> m_proc;
};

#endif // TESTSERVICEPROXY_H
