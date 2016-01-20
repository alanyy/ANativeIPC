#ifndef SERVICEBASEIMPL_H
#define SERVICEBASEIMPL_H

#include <string>
#include "binder/Binder.h"
#include "utils/StrongPointer.h"
#include "ServiceBaseGlobal.h"
#include "ServiceBaseDefines.h"
#include "utils/Vector.h"
#include "utils/Mutex.h"

class ServiceBase;

class ServiceBaseImpl : public android::BBinder
{
public:
    explicit ServiceBaseImpl(const char *name, ServiceBase *interface);
    virtual ~ServiceBaseImpl();

    std::string &name();

    void initialize();
    void finalize();
    void exec();

    virtual android::status_t onTransact(uint32_t code,
                                         const android::Parcel& data,
                                         android::Parcel* reply,
                                         uint32_t flags = 0);

    int addService();
    int removeService();

    int sendAsyncResponse(SenderId &id, unsigned int code,
                          const android::Parcel &data);
private:
    DISABLE_COPY(ServiceBaseImpl)

    android::sp<android::BpBinder> lookupBinder(int handle);
    bool removeBinder(int handle);
    void clearBinders();
    bool addBinder(android::sp<android::BpBinder> binder);
    void onBinderDied(const android::wp<android::IBinder>& who);

    class ClientBinderProxy : public android::IBinder::DeathRecipient
    {
    public:
        ClientBinderProxy(ServiceBaseImpl *service);
        virtual ~ClientBinderProxy();
        virtual void binderDied(const android::wp<android::IBinder>& who);
    private:
        ServiceBaseImpl *m_service;
    };

    std::string m_name;
    ServiceBase *m_if;
    android::Vector<android::sp<android::BpBinder> > m_binders;
    android::Mutex m_clientLock;
    android::sp<ClientBinderProxy> m_clientBinderProxy;
};

#endif // SERVICEBASEIMPL_H
