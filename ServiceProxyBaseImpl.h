#ifndef SERVICEPROXYBASEIMPL_H
#define SERVICEPROXYBASEIMPL_H

#include "binder/IBinder.h"
#include "binder/Parcel.h"
#include "utils/Mutex.h"
#include <utils/String8.h>
#include "ServiceBaseGlobal.h"

class ServiceBinderProxy;
class ServiceProxyBase;

class ServiceProxyBaseImpl : public android::IBinder::DeathRecipient
{
public:
    explicit ServiceProxyBaseImpl(const char *name, ServiceProxyBase *interface);
    virtual ~ServiceProxyBaseImpl();

    virtual void binderDied(const android::wp<android::IBinder>& who);

    android::String8  &name();

    bool tryConnect();

    int sendRequest(unsigned int code, const android::Parcel &data,
                    android::Parcel *reply, unsigned int flags = 0);
private:
    DISABLE_COPY(ServiceProxyBaseImpl)
    ServiceProxyBase *m_if;
    android::sp<ServiceBinderProxy> m_bp;
    android::Mutex m_serviceLock;
    android::String8  m_name;
};

#endif // SERVICEPROXYBASEIMPL_H
