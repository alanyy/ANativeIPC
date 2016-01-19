#ifndef SERVICEPROXYBASE_H
#define SERVICEPROXYBASE_H

#include "binder/Parcel.h"
#include <utils/String8.h>
#include "ServiceBaseGlobal.h"

class ServiceProxyBaseImpl;

class SERVICEBASE_API ServiceProxyBase
{
public:
    explicit ServiceProxyBase(const char *name);
    virtual ~ServiceProxyBase();

    android::String8 name();

    bool isConnected();
    virtual void onConnected();
    virtual void onDisconnected();
protected:
    bool tryConnect();
    int sendRequest(unsigned int code, const android::Parcel &data,
                    android::Parcel *reply, unsigned int flags = 0);

private:
    DISABLE_COPY(ServiceProxyBase)
    android::sp<ServiceProxyBaseImpl> m_impl;
    volatile int m_status;
};

#endif // SERVICEPROXYBASE_H
