#include "ServiceBinderProxy.h"
#include "ServiceBaseDefines.h"
#include "Log.h"

ServiceBinderProxy::ServiceBinderProxy(const android::sp<android::IBinder>& remote)
    : BpRefBase(remote)
{
}

ServiceBinderProxy::~ServiceBinderProxy()
{
}

int ServiceBinderProxy::Transact(unsigned int code,
                                 const android::Parcel &data,
                                 android::Parcel *reply, unsigned int flags)
{
    if(remote()) {
        BSLOGD("ServiceBinderProxy::Transact code is %d", code);
        return remote()->transact(code, data, reply, flags);
    }

    return BS_NO_CONNECTION;
}

int ServiceBinderProxy::linkToDeath(const android::sp<android::IBinder::DeathRecipient> &recipient,
                                    void *cookie, uint32_t flags)
{
    if(remote()) {
        BSLOGD("ServiceBinderProxy::linkToDeath %x", &recipient);
        return remote()->linkToDeath(recipient, cookie, flags);
    }

    return BS_NO_CONNECTION;
}

int ServiceBinderProxy::unlinkToDeath(const android::wp<android::IBinder::DeathRecipient> &recipient,
                                      void *cookie, uint32_t flags,
                                      android::wp<android::IBinder::DeathRecipient> *outRecipient)
{
    if(remote()) {
        BSLOGD("ServiceBinderProxy::unlinkToDeath %x", &recipient);
        return remote()->unlinkToDeath(recipient, cookie, flags, outRecipient);
    }

    return BS_NO_CONNECTION;
}
