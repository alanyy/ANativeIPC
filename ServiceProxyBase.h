#ifndef SERVICEPROXYBASE_H
#define SERVICEPROXYBASE_H

#include "binder/Parcel.h"
#include <string>
#include "ServiceBaseGlobal.h"
#include "ServiceBaseDefines.h"

class ServiceProxyBaseImpl;

class SERVICEBASE_API ServiceProxyBase
{
public:
    explicit ServiceProxyBase(const char *name);
    virtual ~ServiceProxyBase();

    std::string name();

    bool isConnected();
    virtual void onConnected();
    virtual void onDisconnected();

    virtual int onAsyncResponse(unsigned int code, const android::Parcel &reply);
protected:
    bool tryConnect();
    bool setupAsyncRequest();
    bool teardownAsyncRequest();

    int sendSyncRequest(unsigned int code, const android::Parcel &data,
                        android::Parcel *reply);
    bool prepareAsyncData(android::Parcel &data);
    int sendAsyncRequest(unsigned int code, const android::Parcel &data);

private:
    DISABLE_COPY(ServiceProxyBase)
    android::sp<ServiceProxyBaseImpl> m_impl;
    volatile int m_status;
};

#endif // SERVICEPROXYBASE_H
