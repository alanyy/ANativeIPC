#ifndef SERVICEBASE_H
#define SERVICEBASE_H

#include "ServiceBaseGlobal.h"
#include "ServiceBaseDefines.h"
#include "binder/Parcel.h"
#include "utils/String8.h"

class ServiceBaseImpl;

class SERVICEBASE_API ServiceBase
{
public:
    explicit ServiceBase(const char *name);
    virtual ~ServiceBase();

    virtual android::String8 name();

    virtual void initialize();
    virtual void finalize();
    virtual void exec();

    virtual int onSyncRequest(unsigned int code, const android::Parcel &data,
                          android::Parcel *reply);
    virtual int onAsyncRequest(SenderId &id, unsigned int code,
                               const android::Parcel &data);
    virtual void onSenderDisconnect(SenderId &id);

protected:
    virtual int addService();
    virtual int removeService();
    virtual int sendAsyncResponse(SenderId &id, unsigned int code,
                                  const android::Parcel &data);
private:
    DISABLE_COPY(ServiceBase)
    android::sp<ServiceBaseImpl> m_impl;
};

#endif // SERVICEBASE_H
