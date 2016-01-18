#ifndef SERVICEBASEIMPL_H
#define SERVICEBASEIMPL_H

#include <string>
#include "binder/Binder.h"
#include "utils/StrongPointer.h"
#include "ServiceBaseGlobal.h"

namespace android {
class ProcessState;
class IServiceManager;
}
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
private:
    DISABLE_COPY(ServiceBaseImpl)
    std::string m_name;
    ServiceBase *m_if;
    android::sp<android::ProcessState> m_proc;
    android::sp<android::IServiceManager> m_svcMng;
};

#endif // SERVICEBASEIMPL_H
