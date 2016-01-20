#ifndef ANONYMOUSBINDER_H
#define ANONYMOUSBINDER_H

#include "binder/Binder.h"
#include "ServiceBaseDefines.h"

class ServiceProxyBase;

class AnonymousBinder : public android::BBinder
{
public:
    AnonymousBinder(ServiceProxyBase *interface);
    ~AnonymousBinder();

    void setRemoteId(SenderId &id);
    SenderId &remoteId();
    virtual android::status_t onTransact(uint32_t code,
                                         const android::Parcel& data,
                                         android::Parcel* reply,
                                         uint32_t flags = 0);
private:
    SenderId m_remoteId;
    ServiceProxyBase *m_if;
};

#endif // ANONYMOUSBINDER_H
