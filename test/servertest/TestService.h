#ifndef TESTSERVICE_H
#define TESTSERVICE_H

#include "ServiceBase.h"
#include "utils/Condition.h"
#include "ServiceBaseDefines.h"

class TestService : public ServiceBase
{
public:
    enum commandId {
        First_Command = SERVICE_FIRST_TRANSACTION,
        Asyn_Command
    };
    TestService();
    ~TestService();
    virtual int onRequest(unsigned int code, const android::Parcel &data,
                          android::Parcel *reply, unsigned int flags);
    static int asynthread(void *arg);
private:
    static android::sp<android::IBinder> m_binder;
    static android::Condition m_condition;
    static android::Mutex m_lock;
    static bool m_quit;
};

#endif // TESTSERVICE_H
