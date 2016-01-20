#ifndef TESTSERVICE_H
#define TESTSERVICE_H

#include "ServiceBase.h"
#include "utils/Condition.h"
#include "ServiceBaseDefines.h"

class TestService : public ServiceBase
{
public:
    enum commandId {
        Sync_Command = SERVICE_FIRST_TRANSACTION,
        Async_Command
    };
    TestService();
    ~TestService();
//    virtual int onRequest(unsigned int code, const android::Parcel &data,
//                          android::Parcel *reply, unsigned int flags);

    virtual int onSyncRequest(unsigned int code, const android::Parcel &data,
                          android::Parcel *reply);
    virtual int onAsyncRequest(SenderId &id, unsigned int code,
                               const android::Parcel &data);

    void quitAsyncthread();
    static int asyncthread(void *arg);
private:
//    static android::sp<android::IBinder> m_binder;
    static SenderId m_sender;
    static android::Condition m_condition;
    static android::Mutex m_lock;
    static bool m_quit;
};

#endif // TESTSERVICE_H
