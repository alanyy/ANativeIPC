#ifndef SERVICEBINDERPROXY_H
#define SERVICEBINDERPROXY_H
#include "binder/Binder.h"
#include "binder/Parcel.h"
#include "ServiceBaseGlobal.h"

class ServiceBinderProxy : public android::BpRefBase
{
public:
    explicit ServiceBinderProxy(const android::sp<android::IBinder>& remote);
    virtual ~ServiceBinderProxy();

    int Transact(unsigned int code, const android::Parcel& data,
                 android::Parcel* reply, unsigned int flags = 0);
    int linkToDeath(const android::sp<android::IBinder::DeathRecipient>& recipient,
                    void* cookie = NULL,
                    uint32_t flags = 0);
    int unlinkToDeath(const android::wp<android::IBinder::DeathRecipient>& recipient,
                      void* cookie = NULL,
                      uint32_t flags = 0,
                      android::wp<android::IBinder::DeathRecipient>* outRecipient = NULL);
private:
    DISABLE_COPY(ServiceBinderProxy)
};

#endif // SERVICEBINDERPROXY_H
